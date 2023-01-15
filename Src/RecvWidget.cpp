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
    , hexMode_(new QCheckBox("十六进制接收", this))
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
        auto title = new QLabel("接收信息");
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
        auto fileBtn = new QPushButton("存文件");
        ctrl->addWidget(fileBtn);
        ctrl->addWidget(filePath_);
        auto closeBtn = new QPushButton("关闭");
        ctrl->addWidget(closeBtn);
        closeBtn->hide();
    }

    ctrl->addStretch(1);
    {
        auto cleanBtn = new QPushButton("清除");
        ctrl->addWidget(cleanBtn);
    }

    auto layout = new QVBoxLayout(this);
    layout->addLayout(ctrl);
    layout->addWidget(recvEdit_, 1);
}
