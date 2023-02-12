#include "SendWidget.h"
#include "HexUtil.h"
#include "NetworkTaskManager.h"
#include <QCheckBox>
#include <QComboBox>
#include <QFileDialog>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QTextEdit>

SendWidget::SendWidget(QWidget *parent)
    : TitledWidget(tr("Send Information"), new QFrame, new QWidget, parent)
    , mode_(new QComboBox(this))
    , sendEdit_(new QTextEdit(this))
    , intervalBox_(new QCheckBox(tr("Every")))
    , intervalEdit_(new QLineEdit("1000"))
    , autoSend_(new QCheckBox(tr("Start when received any bytes")))
{
    sendEdit_->setAcceptRichText(false);
    intervalEdit_->setFixedWidth(60);
    mode_->addItems(QStringList::fromStdList({ tr("Text"), tr("Hex"), tr("File") }));
    connect(mode_, SIGNAL(currentIndexChanged(int)), this, SLOT(showModeDetail(int)));

    setupUi();
}

void SendWidget::setupUi()
{
    auto ctrl = new QHBoxLayout(corner_);
    ctrl->setContentsMargins(0, 0, 0, 0);
    ctrl->addWidget(mode_);
    {
        auto cleanBtn = new QPushButton(tr("CLEAR"));
        ctrl->addWidget(cleanBtn);
        connect(cleanBtn, SIGNAL(clicked()), sendEdit_, SLOT(clear()));
        connect(cleanBtn, SIGNAL(clicked()), this, SLOT(resetToTextMode()));
    }

    auto sendLayout = new QHBoxLayout;
    {
        sendLayout->addWidget(intervalBox_);
        sendLayout->addWidget(intervalEdit_);
        auto desc = new QLabel(tr("ms send"));
        sendLayout->addWidget(desc);
    }
    sendLayout->addSpacing(20);
    sendLayout->addWidget(autoSend_);
    sendLayout->addStretch(1);
    {
        auto sendBtn = new QPushButton(tr("SEND"));
        sendLayout->addWidget(sendBtn);
        connect(sendBtn, SIGNAL(clicked()), this, SLOT(sendData()), Qt::QueuedConnection);
    }

    auto layout = new QVBoxLayout(central_);
    layout->addWidget(sendEdit_, 1);
    layout->addLayout(sendLayout);
}

void SendWidget::sendData()
{
    auto task = NetworkTaskManager::instance()->current();
    if (task == nullptr)
    {
        return;
    }
    auto data = sendEdit_->toPlainText().toLocal8Bit();

    auto doSend = [task, this](auto &&block) {
        switch (mode_->currentIndex())
        {
        case 0:  // text
            task->send(block);
            break;
        case 1:  // hex
            /// todo
            task->send(fromHexString(QString(block)));
            break;
        case 2:  // file
            /// todo
            break;
        }
    };

    int offset = 0;
    int limit = 4096;
    while (data.size() - offset > limit)
    {
        doSend(data.mid(offset, limit));
        offset += limit;
    }
    doSend(data.mid(offset));
}

void SendWidget::showModeDetail(int index)
{
    if (index == 2)
    {
        sendEdit_->setReadOnly(true);
        openChooseFileDialog();
    }
    else
    {
        sendEdit_->setReadOnly(false);
    }
}

void SendWidget::openChooseFileDialog()
{
    auto path = QFileDialog::getOpenFileName(nullptr, tr("Choose file to send"));
    if (path.isEmpty())
    {
        resetToTextMode();
        return;
    }
    sendEdit_->setText(path);
}

void SendWidget::resetToTextMode()
{
    mode_->setCurrentIndex(0);
}
