#include "ConfigWidget.h"
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

ConfigWidget::ConfigWidget(QWidget *parent)
    : TitledWidget(tr("Configurations"), start_ = new QPushButton(tr("START")), new QWidget, parent)
    , proto_(new QComboBox(this))
    , localIp_(new QComboBox(this))
    , localPort_(new QLineEdit("44441", this))
    , protoDetail_(new QStackedWidget(this))
    , form_(new QWidget)
{
    qRegisterMetaType<NetworkConfig>("NetworkConfig");

    start_->setObjectName("ctrl");
    start_->setFixedSize(80, 32);
    start_->setCheckable(true);

    localIp_->addItem("0.0.0.0");
    localIp_->addItems(getLocalIpAddress());
    registerProto<TcpServerTask, QWidget>("TCP Server");
    registerProto<TcpClientTask, RemoteHostWidget>("TCP Client");
    registerProto<UdpTask, RemoteHostWidget>("UDP");
    registerProto<MulticastTask, MulticastConfigWidget>("UDP Multicast");

    setupUi();
}

void ConfigWidget::setupUi()
{
    auto row1 = new QHBoxLayout(central_);
    row1->setContentsMargins(10, 10, 10, 10);
    row1->addLayout(makeLabeledField(tr("Protocol"), proto_));
    row1->addLayout(makeLabeledField(tr("Local IP"), localIp_));
    row1->addLayout(makeLabeledField(tr("Local Port"), localPort_, 60));
    row1->addWidget(protoDetail_);
    row1->addStretch(1);
    row1->addSpacing(20);
    row1->addWidget(start_);

    connect(proto_, SIGNAL(currentIndexChanged(int)), protoDetail_, SLOT(setCurrentIndex(int)));
    connect(start_, SIGNAL(clicked(bool)), this, SLOT(ctrlNetworkTask(bool)));
}

void ConfigWidget::startNewNetworkTask()
{
    auto ntm = NetworkTaskManager::instance();

    auto detail = protoDetail_->currentWidget();
    auto nc = gatherConfig(detail);
    auto task = ntm->create(proto_->currentText(), nc);
    connect(task, SIGNAL(workStateChanged(int)), this, SLOT(updateCtrlButtons(int)));
    Q_CHECK_PTR(task);
    ntm->start(task);

    QMetaObject::invokeMethod(detail, "onTaskStarted", Qt::DirectConnection);
}

void ConfigWidget::stopCurrentNetworkTask()
{
    auto detail = protoDetail_->currentWidget();
    QMetaObject::invokeMethod(detail, "onTaskStopped", Qt::DirectConnection);
    form_->setEnabled(true);
    start_->setEnabled(true);
    start_->setText(tr("START"));
    start_->setChecked(false);
    NetworkTaskManager::instance()->stopCurrent();
}

NetworkConfig ConfigWidget::gatherConfig(QWidget *detail) const
{
    NetworkConfig nc;
    nc.protocol = proto_->currentText();
    nc.localIp = localIp_->currentText();
    nc.localPort = localPort_->text().toInt();

    QMetaObject::invokeMethod(detail, "updateConfig", Qt::DirectConnection, Q_ARG(NetworkConfig &, nc));
    return nc;
}

void ConfigWidget::ctrlNetworkTask(bool checked)
{
    form_->setDisabled(true);
    start_->setDisabled(true);

    if (checked)
    {
        try
        {
            startNewNetworkTask();
        }
        catch (const std::exception &e)
        {
            QMessageBox::critical(nullptr, tr("Error"), QString::fromStdString(e.what()));
            updateCtrlButtons(NetworkTask::FAILED);
            return;
        }
    }
    else
    {
        stopCurrentNetworkTask();
    }
}

void ConfigWidget::updateCtrlButtons(int state)
{
    if (state == NetworkTask::OK)
    {
        start_->setText(tr("STOP"));
        start_->setChecked(true);
    }
    else
    {
        stopCurrentNetworkTask();
    }
    start_->setEnabled(true);
}
