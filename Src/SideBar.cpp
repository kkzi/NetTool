#include "SideBar.h"
#include "CommonUi.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QComboBox>
#include <QStackedWidget>

SideBar::SideBar(QWidget* parent)
    : QWidget(parent)
    , proto_(new QComboBox(this))
    , ip_(new QComboBox(this))
    , port_(new QLineEdit(this))
    , protoDetail_(new QStackedWidget(this))
    , start_(new QPushButton("开始", this))
{
    start_->setFixedWidth(80);
    setupUi();
}

void SideBar::registerProto(QString name, QWidget* detail)
{
    proto_->addItem(name);
    protoDetail_->addWidget(detail);
}

void SideBar::setupUi()
{
    auto ly = new QVBoxLayout(this);
    {
        auto logo = new QLabel("LOGO");
        ly->addWidget(logo);
        ly->addLayout(makeFormRow("协议类型", proto_));
        ly->addLayout(makeFormRow("本地IP", ip_));
        ly->addLayout(makeFormRow("本地端口", port_));
        ly->addWidget(protoDetail_);
        ly->addWidget(start_);
    }
    ly->addStretch(1);
    connect(proto_, SIGNAL(currentIndexChanged(int)), protoDetail_, SLOT(setCurrentIndex(int)));
}
