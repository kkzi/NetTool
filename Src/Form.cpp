#include "Form.h"
#include "LogWidget.h"
#include "NetworkTaskManager.h"
#include "RecvWidget.h"
#include "SendWidget.h"
#include "SideBar.h"
#include <QHBoxLayout>
#include <QLabel>
#include <QStatusBar>
#include <QTextEdit>

Form::Form()
    : side_(new SideBar(this))
    , recv_(new RecvWidget(this))
    , send_(new SendWidget(this))
    , logWidget_(new LogWidget(this))
{
    setupUi();

    auto ntm = NetworkTaskManager::instance();
    connect(ntm, SIGNAL(logMessage(QString)), logWidget_, SLOT(append(QString)), Qt::QueuedConnection);
    connect(
        ntm, SIGNAL(dataReceived(QString, QByteArray)), recv_, SLOT(append(QString, QByteArray)), Qt::QueuedConnection);
}

Form::~Form()
{
    NetworkTaskManager::instance()->stopCurrent();
}

void Form::setupUi()
{
    side_->setMinimumWidth(180);
    side_->setMaximumWidth(400);

    auto central = new QFrame;
    auto vbox = new QVBoxLayout(central);
    vbox->addWidget(recv_, 3);
    vbox->addWidget(send_, 1);

    auto hbox = new QHBoxLayout;
    hbox->addWidget(side_);
    hbox->addWidget(central, 1);

    auto layout = new QVBoxLayout(this);
    layout->setMargin(0);
    layout->addLayout(hbox, 1);
    layout->addWidget(logWidget_);
    //layout->addWidget(new QStatusBar);
}
