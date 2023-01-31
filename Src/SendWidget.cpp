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
    , intervalBox_(new QCheckBox("ÿ��"))
    , intervalEdit_(new QLineEdit("1000"))
    , autoSend_(new QCheckBox("�յ��κ����ݺ�ʼ����"))
    , sendEdit_(new QTextEdit)
    , filePath_(new QLabel)
{
    bufferLimit_->setFixedWidth(50);
    mode_->addItems(QStringList::fromStdList({ "�ı�", "ʮ������", "�ļ�" }));
    connect(mode_, SIGNAL(currentIndexChanged(int)), this, SLOT(showModeDetail(int)));

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
    {
        ctrl->addWidget(new QLabel("��������С"));
        ctrl->addWidget(bufferLimit_);
        ctrl->addWidget(new QLabel("�ֽ�"));
    }
    ctrl->addWidget(mode_);

    //{
    //    auto fileBtn = new QPushButton("ѡ���ļ�");
    //    ctrl->addWidget(fileBtn);
    //    ctrl->addWidget(filePath_);
    //    auto closeBtn = new QPushButton("�ر�");
    //    ctrl->addWidget(closeBtn);
    //    closeBtn->hide();
    //}

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
