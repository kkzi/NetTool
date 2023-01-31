#include "SendWidget.h"
#include "HexUtil.h"
#include "NetworkTaskManager.h"
#include <QCheckBox>
#include <QComboBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QTextEdit>

SendWidget::SendWidget(QWidget *parent)
    : QFrame(parent)
    , mode_(new QComboBox(this))
    , bufferLimit_(new QLineEdit("1000"))
    , intervalBox_(new QCheckBox("每隔"))
    , intervalEdit_(new QLineEdit("1000"))
    , autoSend_(new QCheckBox("收到任何数据后开始发送"))
    , sendEdit_(new QTextEdit)
    , filePath_(new QLabel)
{
    bufferLimit_->setFixedWidth(50);
    mode_->addItems(QStringList::fromStdList({ "文本", "十六进制", "文件" }));
    connect(mode_, SIGNAL(currentIndexChanged(int)), this, SLOT(showModeDetail(int)));

    setupUi();
}

void SendWidget::setupUi()
{
    auto ctrl = new QHBoxLayout;
    {
        auto title = new QLabel("发送信息");
        title->setObjectName("title");
        ctrl->addWidget(title);
    }
    {
        ctrl->addWidget(new QLabel("缓冲区大小"));
        ctrl->addWidget(bufferLimit_);
        ctrl->addWidget(new QLabel("字节"));
    }
    ctrl->addWidget(mode_);

    //{
    //    auto fileBtn = new QPushButton("选择文件");
    //    ctrl->addWidget(fileBtn);
    //    ctrl->addWidget(filePath_);
    //    auto closeBtn = new QPushButton("关闭");
    //    ctrl->addWidget(closeBtn);
    //    closeBtn->hide();
    //}

    ctrl->addStretch(1);

    auto send = new QHBoxLayout;
    {
        send->addWidget(intervalBox_);
        send->addWidget(intervalEdit_);
        auto desc = new QLabel("毫秒发送");
        send->addWidget(desc);
    }
    send->addWidget(autoSend_);
    {
        auto sendBtn = new QPushButton("发送");
        send->addWidget(sendBtn);
        connect(sendBtn, SIGNAL(clicked()), this, SLOT(sendData()), Qt::QueuedConnection);
    }

    auto layout = new QVBoxLayout(this);
    layout->addLayout(ctrl);
    layout->addWidget(sendEdit_, 1);
    layout->addLayout(send);
}

void SendWidget::sendData()
{
    auto task = NetworkTaskManager::instance()->current();
    if (task == nullptr)
    {
        return;
    }
    auto data = sendEdit_->toPlainText().toLocal8Bit();

    switch (mode_->currentIndex())
    {
    case 0:  // text
        task->send(data);
        break;
    case 1:  // hex
        /// todo
        task->send(fromHexString(QString(data)));
        break;
    case 2:  // file
        /// todo
        break;
    }
}

void SendWidget::showModeDetail(int)
{
    // do nothing
}
