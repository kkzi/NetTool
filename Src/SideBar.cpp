#include "SideBar.h"
#include "CommonUi.h"
#include "NetUtil.h"
#include "NetworkTask.h"
#include <QComboBox>
#include <QLabel>
#include <QLineEdit>
#include <QMetaObject>
#include <QPushButton>
#include <QStackedWidget>
#include <QVBoxLayout>

SideBar::SideBar(QWidget *parent)
    : QWidget(parent)
    , io_(1)
    , form_(new QWidget)
    , proto_(new QComboBox(this))
    , localIp_(new QComboBox(this))
    , localPort_(new QLineEdit(this))
    , protoDetail_(new QStackedWidget(this))
    , start_(new QPushButton("��ʼ", this))
{
    qRegisterMetaType<NetworkConfig>("NetworkConfig");

    start_->setFixedWidth(80);
    start_->setCheckable(true);
    setupUi();

    localIp_->addItems(getLocalIpAddress());
}

void SideBar::setupUi()
{
    auto form = new QVBoxLayout(form_);
    form->setContentsMargins(0, 0, 0, 0);
    {
        auto logo = new QLabel("LOGO");
        form->addWidget(logo);
        form->addLayout(makeFormRow("Э������", proto_));
        form->addLayout(makeFormRow("���ص�ַ", localIp_));
        form->addLayout(makeFormRow("���ض˿�", localPort_));
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
    Q_ASSERT(current_ == nullptr);

    auto nc = gatherConfig();

    auto task = NetworkTaskFactory::create(proto_->currentText());
    Q_CHECK_PTR(task);
    task->setConfig(nc);
    task->start(io_);
    current_ = task;

    thread_ = std::thread([this] {
        io_.run();
    });
}

void SideBar::stopCurrentNetworkTask()
{
    io_.stop();
    if (thread_.joinable())
    {
        thread_.join();
    }

    Q_CHECK_PTR(current_);
    delete current_;
    current_ = nullptr;
}

NetworkConfig SideBar::gatherConfig() const
{
    NetworkConfig nc;
    nc.protocol = proto_->currentText();
    nc.localIp = localIp_->currentText();
    nc.localPort = localPort_->text().toInt();

    auto detail = protoDetail_->currentWidget();
    QMetaObject::invokeMethod(detail, "updateConfig", Qt::DirectConnection, Q_ARG(NetworkConfig &, nc));
    return nc;
}

void SideBar::ctrlNetworkTask(bool checked)
{
    form_->setDisabled(checked);
    if (checked)
    {
        startNewNetworkTask();
        start_->setText("ֹͣ");
    }
    else
    {
        stopCurrentNetworkTask();
        start_->setText("��ʼ");
    }
}
