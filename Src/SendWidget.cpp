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
    , frameLimitEdit_(new QLineEdit("0"))
    , autoSend_(new QCheckBox(tr("Start when received any bytes")))
{
    sendEdit_->setAcceptRichText(false);
    intervalEdit_->setFixedWidth(60);
    frameLimitEdit_->setFixedWidth(60);
    mode_->addItems(QStringList{ tr("Text"), tr("Hex"), tr("File") });
    connect(mode_, &QComboBox::currentIndexChanged, this, &SendWidget::showModeDetail);
    connect(intervalBox_, &QCheckBox::clicked, this, &SendWidget::ctrlSendTimer);

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
        connect(cleanBtn, &QPushButton::clicked, sendEdit_, &QTextEdit::clear);
        connect(cleanBtn, &QPushButton::clicked, this, &SendWidget::resetToTextMode);
    }

    auto sendLayout = new QHBoxLayout;
    {
        sendLayout->addWidget(intervalBox_);
        sendLayout->addWidget(intervalEdit_);
        sendLayout->addWidget(new QLabel(tr("ms send")));
        sendLayout->addWidget(frameLimitEdit_);
        sendLayout->addWidget(new QLabel(tr("bytes")));
    }
    sendLayout->addSpacing(20);
    /// todo
    // sendLayout->addWidget(autoSend_);
    autoSend_->hide();

    sendLayout->addStretch(1);
    {
        sendButton_ = new QPushButton(tr("SEND"));
        sendLayout->addWidget(sendButton_);
        connect(sendButton_, &QPushButton::clicked, this, &SendWidget::sendData, Qt::QueuedConnection);
    }

    auto layout = new QVBoxLayout(central_);
    layout->addWidget(sendEdit_, 1);
    layout->addLayout(sendLayout);
}

QByteArray SendWidget::prepareData()
{
    if (isFileMode())
    {
        QFile file(sendEdit_->toPlainText());
        if (!file.open(QFile::ReadOnly))
        {
            return {};
        }
        file.seek(offset_);
        auto len = frameLimitEdit_->text().toInt();
        len = len > 0 ? len : file.size();
        auto bytes = file.read(len);
        file.close();
        offset_ += len;
        if (offset_ >= file.size())
        {
            offset_ = 0;
        }
        return bytes;
    }
    else
    {
        return sendEdit_->toPlainText().toLocal8Bit();
    }
}

bool SendWidget::isFileMode() const
{
    return mode_->currentIndex() == 2;
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

    switch (mode_->currentIndex())
    {
    case 0:  // text
    case 2:  // file
        task->send(data);
        break;
    case 1:  // hex
        task->send(fromHexString(QString(data)));
        break;
    }
}

void SendWidget::showModeDetail(int index)
{
    if (isFileMode())
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
    sendButton_->setDisabled(true);
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

    offset_ = 0;
    if (checked)
    {
        timer_ = new QTimer(this);
        timer_->setSingleShot(false);
        timer_->setInterval(ms);
        timer_->start();
        connect(timer_, &QTimer::timeout, this, &SendWidget::sendData, Qt::UniqueConnection);
        sendButton_->setDisabled(true);
        intervalEdit_->setDisabled(true);
        frameLimitEdit_->setDisabled(true);
    }
    else if (timer_ != nullptr)
    {
        timer_->stop();
        delete timer_;
        timer_ = nullptr;
        sendButton_->setEnabled(true);
        intervalEdit_->setEnabled(true);
        frameLimitEdit_->setEnabled(true);
    }
}

void SendWidget::resetToTextMode()
{
    mode_->setCurrentIndex(0);
    sendButton_->setEnabled(true);
}
