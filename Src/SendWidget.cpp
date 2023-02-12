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
#include <QTimer>

SendWidget::SendWidget(QWidget *parent)
    : TitledWidget(tr("Send Information"), new QFrame, new QWidget, parent)
    , mode_(new QComboBox(this))
    , sendEdit_(new QTextEdit(this))
    , intervalBox_(new QCheckBox(tr("Every")))
    , intervalEdit_(new QLineEdit("1000"))
    , frameLimitEdit_(new QLineEdit("512"))
    , autoSend_(new QCheckBox(tr("Start when received any bytes")))
{
    sendEdit_->setAcceptRichText(false);
    intervalEdit_->setFixedWidth(60);
    frameLimitEdit_->setFixedWidth(60);
    mode_->addItems(QStringList::fromStdList({ tr("Text"), tr("Hex"), tr("File") }));
    connect(mode_, SIGNAL(currentIndexChanged(int)), this, SLOT(showModeDetail(int)));
    connect(intervalBox_, SIGNAL(clicked(bool)), this, SLOT(ctrlSendTimer(bool)));

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
    {
        sendLayout->addWidget(new QLabel(tr("Frame Bytes")));
        sendLayout->addWidget(frameLimitEdit_);
    }
    sendLayout->addSpacing(20);
    /// todo
    // sendLayout->addWidget(autoSend_);
    autoSend_->hide();

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

QByteArray SendWidget::prepareData() const
{
    if (auto isFileMode = mode_->currentIndex() == 2; isFileMode)
    {
        QFile file(sendEdit_->toPlainText());
        if (!file.open(QFile::ReadOnly))
        {
            return {};
        }
        /// todo
        /// large file maybe lag/slow
        auto bytes = file.readAll();
        file.close();
        return bytes;
    }
    else
    {
        return sendEdit_->toPlainText().toLocal8Bit();
    }
}

void SendWidget::sendData()
{
    auto task = NetworkTaskManager::instance()->current();
    if (task == nullptr)
    {
        return;
    }
    auto data = prepareData();
    if (data.isEmpty())
    {
        return;
    }

    auto doSend = [task, this](auto &&block) {
        switch (mode_->currentIndex())
        {
        case 0:  // text
        case 2:  // file
            task->send(block);
            break;
        case 1:  // hex
            task->send(fromHexString(QString(block)));
            break;
        }
    };

    /// todo
    int offset = 0;
    int limit = frameLimitEdit_->text().toInt();
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

void SendWidget::ctrlSendTimer(bool checked)
{
    bool ok = false;
    auto ms = intervalEdit_->text().toInt(&ok);
    if (!ok)
    {
        intervalBox_->setChecked(false);
        intervalEdit_->setFocus();
        return;
    }

    if (checked)
    {
        timer_ = new QTimer(this);
        timer_->setSingleShot(false);
        timer_->setInterval(ms);
        timer_->start();
        connect(timer_, SIGNAL(timeout()), this, SLOT(sendData()));
        intervalEdit_->setDisabled(true);
    }
    else if (timer_ != nullptr)
    {
        timer_->stop();
        delete timer_;
        timer_ = nullptr;
        intervalEdit_->setDisabled(false);
    }
}

void SendWidget::resetToTextMode()
{
    mode_->setCurrentIndex(0);
}
