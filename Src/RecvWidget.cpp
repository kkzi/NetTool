#include "RecvWidget.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QTextEdit>
#include <QCheckBox>
#include <QComboBox>
#include <QPushButton>

RecvWidget::RecvWidget(QWidget* parent)
    : QFrame(parent)
    , recvEdit_(new QTextEdit(this))
    , hexMode_(new QCheckBox("ʮ�����ƽ���", this))
    , filePath_(new QLabel(this))
    , bufferLimit_(new QLineEdit("1000", this))
{
    recvEdit_->setReadOnly(true);
    setupUi();
}

void RecvWidget::setupUi()
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
        auto fileBtn = new QPushButton("���ļ�");
        ctrl->addWidget(fileBtn);
        ctrl->addWidget(filePath_);
        auto closeBtn = new QPushButton("�ر�");
        ctrl->addWidget(closeBtn);
        closeBtn->hide();
    }

    ctrl->addStretch(1);
    {
        auto cleanBtn = new QPushButton("���");
        ctrl->addWidget(cleanBtn);
    }

    auto layout = new QVBoxLayout(this);
    layout->addLayout(ctrl);
    layout->addWidget(recvEdit_, 1);
}
