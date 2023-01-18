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
    auto layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);

    layout->addLayout(makeFormRow("Ŀ�ĵ�ַ", remoteIp_));
    layout->addLayout(makeFormRow("Ŀ�Ķ˿�", remotePort_));
    layout->addStretch();
}

void RemoteHostWidget::updateConfig(NetworkConfig &nc)
{
    nc.remoteIp = remoteIp_->text();
    nc.remotePort = remotePort_->text().toInt();
}
