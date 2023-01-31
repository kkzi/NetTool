#include "SideBar.h"
#include "CommonUi.h"
#include "MulticastConfigWidget.h"
#include "MulticastTask.h"
#include "NetUtil.h"
#include "NetworkTask.h"
#include "RemoteHostWidget.h"
#include "TcpClientTask.h"
#include "TcpServerTask.h"
#include "TcpSessionWidget.h"
#include "UdpTask.h"
#include <QComboBox>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QMetaObject>
#include <QPushButton>
#include <QStackedWidget>
#include <QVBoxLayout>

SideBar::SideBar(QWidget *parent)
    : QWidget(parent)
    , form_(new QWidget)
    , proto_(new QComboBox(this))
    , localIp_(new QComboBox(this))
    , localPort_(new QLineEdit("44441", this))
    , protoDetail_(new QStackedWidget(this))
    , start_(new QPushButton("开始", this))
{
    qRegisterMetaType<NetworkConfig>("NetworkConfig");

    setupUi();

    start_->setFixedWidth(80);
    start_->setCheckable(true);

    localIp_->addItem("0.0.0.0");
    localIp_->addItems(getLocalIpAddress());
    registerProto<TcpServerTask, TcpSessionWidget>("TCP Server");
    registerProto<TcpClientTask, RemoteHostWidget>("TCP Client");
    registerProto<UdpTask, RemoteHostWidget>("UDP");
    registerProto<MulticastTask, MulticastConfigWidget>("UDP Multicast");
}

void SideBar::setupUi()
{
    auto form = new QVBoxLayout(form_);
    form->setContentsMargins(0, 0, 0, 0);
    {
        auto logo = new QLabel("LOGO");
        form->addWidget(logo);
        form->addLayout(makeFormRow("协议类型", proto_));
        form->addLayout(makeFormRow("本地地址", localIp_));
        form->addLayout(makeFormRow("本地端口", localPort_));
        form->addWidget(protoDetail_);
    }

    auto layout = new QVBoxLayout(this);
    layout->addWidget(form_);
    layout->addWidget(start_);
    layout->addStretch(1);

    connect(proto_, SIGNAL(currentIndexChanged(int)), protoDetail_, SLOT(setCurrentIndex(int)));
    connect(start_, SIGNAL(clicked(bool)), this, SLOT(ctrlNetworkTask(bool)));
}

void SideBar::startNewNetworkTask()
{
    auto detail = protoDetail_->currentWidget();
    auto nc = gatherConfig(detail);
    auto task = NetworkTaskManager::instance()->start(proto_->currentText(), nc);
    Q_CHECK_PTR(task);
    QMetaObject::invokeMethod(detail, "onTaskStarted", Qt::DirectConnection);
}

void SideBar::stopCurrentNetworkTask()
{
    auto detail = protoDetail_->currentWidget();
    QMetaObject::invokeMethod(detail, "onTaskStopped", Qt::DirectConnection);

    NetworkTaskManager::instance()->stopCurrent();
}

NetworkConfig SideBar::gatherConfig(QWidget *detail) const
{
    NetworkConfig nc;
    nc.protocol = proto_->currentText();
    nc.localIp = localIp_->currentText();
    nc.localPort = localPort_->text().toInt();

    QMetaObject::invokeMethod(detail, "updateConfig", Qt::DirectConnection, Q_ARG(NetworkConfig &, nc));
    return nc;
}

void SideBar::ctrlNetworkTask(bool checked)
{
    if (checked)
    {
        try
        {
            form_->setDisabled(true);
            startNewNetworkTask();
            start_->setText("停止");
        }
        catch (const std::exception &e)
        {
            QMessageBox::critical(nullptr, "错误", QString::fromStdString(e.what()));
            form_->setEnabled(true);
            return;
        }
    }
    else
    {
        stopCurrentNetworkTask();
        start_->setText("开始");
        form_->setEnabled(true);
    }
}
