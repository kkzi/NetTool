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
    , hexMode_(new QCheckBox("ʮ�����Ʒ���"))
    , bufferLimit_(new QLineEdit("1000"))
    , intervalBox_(new QCheckBox("ÿ��"))
    , intervalEdit_(new QLineEdit("1000"))
    , autoSend_(new QCheckBox("�յ��κ����ݺ�ʼ����"))
    , sendEdit_(new QTextEdit)
    , filePath_(new QLabel)
{
    setupUi();
}

void SendWidget::setupUi()
{
    auto ctrl = new QHBoxLayout;
    {
        auto title = new QLabel("������Ϣ");
        title->setObjectName("title");
        ctrl->addWidget(title);
    }
    ctrl->addWidget(hexMode_);
    {
        ctrl->addWidget(new QLabel("��������С"));
        ctrl->addWidget(bufferLimit_);
        ctrl->addWidget(new QLabel("�ֽ�"));
    }
    {
        auto fileBtn = new QPushButton("ѡ���ļ�");
        ctrl->addWidget(fileBtn);
        ctrl->addWidget(filePath_);
        auto closeBtn = new QPushButton("�ر�");
        ctrl->addWidget(closeBtn);
        closeBtn->hide();
    }

    ctrl->addStretch(1);

    auto send = new QHBoxLayout;
    {
        send->addWidget(intervalBox_);
        send->addWidget(intervalEdit_);
        auto desc = new QLabel("���뷢��");
        send->addWidget(desc);
    }
    send->addWidget(autoSend_);
    {
        auto sendBtn = new QPushButton("����");
        send->addWidget(sendBtn);
    }

    auto layout = new QVBoxLayout(this);
    layout->addLayout(ctrl);
    layout->addWidget(sendEdit_, 1);
    layout->addLayout(send);
}

