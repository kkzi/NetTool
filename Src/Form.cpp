#include "Form.h"
#include "MulticastConfigWidget.h"
#include "MulticastTask.h"
#include "RecvWidget.h"
#include "RemoteHostWidget.h"
#include "SendWidget.h"
#include "SideBar.h"
#include "TcpClientTask.h"
#include "TcpServerTask.h"
#include "UdpTask.h"
#include <QHBoxLayout>
#include <QLabel>
#include <QStatusBar>

Form::Form()
    : side_(new SideBar(this))
{
    setupUi();

    side_->registerProto<TcpServerTask, QWidget>("TCP Server");
    side_->registerProto<TcpClientTask, RemoteHostWidget>("TCP Client");
    side_->registerProto<UdpTask, RemoteHostWidget>("UDP");
    side_->registerProto<MulticastTask, MulticastConfigWidget>("Multicast");
}

void Form::setupUi()
{
    side_->setMinimumWidth(180);
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
