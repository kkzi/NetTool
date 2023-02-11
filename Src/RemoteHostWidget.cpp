#include "RemoteHostWidget.h"
#include "CommonUi.h"
#include "NetworkConfig.h"
#include <QLabel>
#include <QLineEdit>
#include <QString>
#include <QVBoxLayout>
#include <QWidget>

RemoteHostWidget::RemoteHostWidget(QWidget *parent /*= nullptr*/)
    : QWidget(parent)
    , remoteIp_(new QLineEdit)
    , remotePort_(new QLineEdit)
{

    /// todo for test
    remoteIp_->setText("127.0.0.1");
    remotePort_->setText("33001");
    ///

    auto layout = new QHBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);

    layout->addLayout(makeLabeledField(tr("Remote IP"), remoteIp_));
    layout->addLayout(makeLabeledField(tr("Remote Port"), remotePort_, 60));
    layout->addStretch();
}

void RemoteHostWidget::updateConfig(NetworkConfig &nc)
{
    nc.remoteIp = remoteIp_->text();
    nc.remotePort = remotePort_->text().toInt();
}
