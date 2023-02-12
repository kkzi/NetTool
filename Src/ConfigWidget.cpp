#include "ConfigWidget.h"
#include "AppSettingsDialog.h"
#include "CommonUi.h"
#include "MulticastConfigWidget.h"
#include "MulticastTask.h"
#include "NetUtil.h"
#include "NetworkTask.h"
#include "PlaceholderWidget.h"
#include "RemoteHostWidget.h"
#include "TcpClientTask.h"
#include "TcpServerTask.h"
#include "TcpSessionWidget.h"
#include "UdpTask.h"
#include <QComboBox>
#include <QEvent>
#include <QIcon>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QMetaObject>
#include <QPushButton>
#include <QSettings>
#include <QStackedWidget>
#include <QToolButton>
#include <QVBoxLayout>

ConfigWidget::ConfigWidget(QWidget *parent)
    : TitledWidget(tr("Configurations"), new QWidget, new QWidget, parent)
    , proto_(new QComboBox(this))
    , localIp_(new QComboBox(this))
    , localPort_(new QLineEdit("44441", this))
    , protoDetail_(new QStackedWidget(this))
    , form_(new QWidget)
{
    qRegisterMetaType<NetworkConfig>("NetworkConfig");

    start_ = new QPushButton(tr("START"));
    start_->setObjectName("ctrl");
    start_->setFixedSize(80, 32);
    start_->setCheckable(true);

    auto ips = getLocalIpAddress();
    localIp_->addItems(ips);

    registerProto<TcpServerTask, PlaceholderWidget>("TCP Server");
    registerProto<TcpClientTask, RemoteHostWidget>("TCP Client");
    registerProto<UdpTask, RemoteHostWidget>("UDP");
    registerProto<MulticastTask, MulticastConfigWidget>("UDP Multicast");

    setupUi();
    {
        QSettings s(CONFIG_FILE_NAME, QSettings::IniFormat);
        proto_->setCurrentIndex(s.value(KEY_PROTOCOL).toInt());
    }
}

void ConfigWidget::setupUi()
{
    {
        auto cornerLayout = new QHBoxLayout(corner_);
        cornerLayout->setContentsMargins(0, 0, 0, 0);
        auto settingsBtn = new QToolButton(this);
        settingsBtn->setIcon(QIcon(":/img/settings.png"));
        settingsBtn->setText(tr("Settings"));
        settingsBtn->setToolButtonStyle(Qt::ToolButtonIconOnly);
        settingsBtn->setObjectName("lang");
        settingsBtn->setIconSize(QSize(22, 22));
        settingsBtn->setFixedSize(settingsBtn->iconSize());
        cornerLayout->addWidget(settingsBtn);
        connect(settingsBtn, SIGNAL(clicked()), this, SLOT(showSettingsDialog()));
    }
    {
        auto formLayout = new QHBoxLayout(form_);
        formLayout->setContentsMargins(10, 10, 10, 10);
        formLayout->addLayout(makeLabeledField(tr("Protocol"), proto_));
        formLayout->addLayout(makeLabeledField(tr("Local IP"), localIp_));
        formLayout->addLayout(makeLabeledField(tr("Local Port"), localPort_, 60));
        formLayout->addWidget(protoDetail_);

        auto centralLayout = new QHBoxLayout(central_);
        centralLayout->addWidget(form_);
        centralLayout->addStretch(1);
        centralLayout->addSpacing(20);
        centralLayout->addWidget(start_);
    }

    connect(proto_, SIGNAL(currentIndexChanged(int)), this, SLOT(showDetailWidget(int)));
    connect(start_, SIGNAL(clicked(bool)), this, SLOT(ctrlNetworkTask(bool)));
}

void ConfigWidget::startNewNetworkTask()
{
    auto proto = proto_->currentText();
    {
        QSettings s(CONFIG_FILE_NAME, QSettings::IniFormat);
        s.setValue(KEY_PROTOCOL, proto_->currentIndex());
        s.beginGroup(proto);
        s.setValue(KEY_LOCAL_IP, localIp_->currentIndex());
        s.setValue(KEY_LOCAL_PORT, localPort_->text());
        s.endGroup();
    }

    auto ntm = NetworkTaskManager::instance();
    auto detail = protoDetail_->currentWidget();
    auto nc = gatherConfig(detail);
    auto task = ntm->create(proto, nc);
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

void ConfigWidget::showDetailWidget(int index)
{
    {
        QSettings s(CONFIG_FILE_NAME, QSettings::IniFormat);
        auto proto = proto_->currentText();
        s.beginGroup(proto);
        auto ip = s.value(KEY_LOCAL_IP).toInt();
        localIp_->setCurrentIndex(ip);
        localPort_->setText(s.value(KEY_LOCAL_PORT).toString());
        s.endGroup();
    }
    protoDetail_->setCurrentIndex(index);
}

void ConfigWidget::showSettingsDialog()
{
    AppSettingsDialog dialog(this);
    dialog.exec();
}
