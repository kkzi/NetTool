#include "Form.h"
#include "SideBar.h"
#include "SendWidget.h"
#include "RecvWidget.h"
#include <QHBoxLayout>
#include <QLabel>
#include <QStatusBar>


Form::Form()
    : side_(new SideBar(this))
{
    setupUi();

    side_->registerProto("TCP Server", new QLabel("todo"));
    side_->registerProto("TCP Client", new QLabel("todo"));
    side_->registerProto("UDP", new QLabel("todo"));
    side_->registerProto("Multicast", new QLabel("todo1"));
}

void Form::setupUi()
{
    side_->setMinimumWidth(150);
    side_->setMaximumWidth(400);

    auto central = new QFrame;
    auto vbox = new QVBoxLayout(central);
    vbox->addWidget(new RecvWidget(this));
    vbox->addWidget(new SendWidget(this));

    auto hbox = new QHBoxLayout;
    hbox->addWidget(side_);
    hbox->addWidget(central, 1);

    auto layout = new QVBoxLayout(this);
    layout->setMargin(0);
    layout->addLayout(hbox, 1);
    layout->addWidget(new QStatusBar);
}
