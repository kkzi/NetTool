#include "SendWidget.h"

#include <QCheckBox>
#include <QLineEdit>
#include <QTextEdit>
#include <QCheckBox>
#include <QLabel>
#include <QHBoxLayout>
#include <QPushButton>


SendWidget::SendWidget(QWidget* parent)
    : QFrame(parent)
    , hexMode_(new QCheckBox("十六进制发送"))
    , bufferLimit_(new QLineEdit("1000"))
    , intervalBox_(new QCheckBox("每隔"))
    , intervalEdit_(new QLineEdit("1000"))
    , autoSend_(new QCheckBox("收到任何数据后开始发送"))
    , sendEdit_(new QTextEdit)
    , filePath_(new QLabel)
{
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
    ctrl->addWidget(hexMode_);
    {
        ctrl->addWidget(new QLabel("缓冲区大小"));
        ctrl->addWidget(bufferLimit_);
        ctrl->addWidget(new QLabel("字节"));
    }
    {
        auto fileBtn = new QPushButton("选择文件");
        ctrl->addWidget(fileBtn);
        ctrl->addWidget(filePath_);
        auto closeBtn = new QPushButton("关闭");
        ctrl->addWidget(closeBtn);
        closeBtn->hide();
    }

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
    }

    auto layout = new QVBoxLayout(this);
    layout->addLayout(ctrl);
    layout->addWidget(sendEdit_, 1);
    layout->addLayout(send);
}

