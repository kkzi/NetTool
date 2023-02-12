#include "RemoteHostWidget.h"
#include "CommonUi.h"
#include "NetworkConfig.h"
#include <QLabel>
#include <QLineEdit>
#include <QSettings>
#include <QString>
#include <QVBoxLayout>
#include <QWidget>

RemoteHostWidget::RemoteHostWidget(const QString &configGroup, QWidget *parent /*= nullptr*/)
    : QWidget(parent)
    , configGroup_(configGroup)
    , remoteIp_(new QLineEdit)
    , remotePort_(new QLineEdit)
{
    {
        QSettings s(CONFIG_FILE_NAME, QSettings::IniFormat);
        s.beginGroup(configGroup_);
        remoteIp_->setText(s.value(KEY_REMOTE_IP).toString());
        remotePort_->setText(s.value(KEY_REMOTE_PORT).toString());
        s.endGroup();
    }

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
    {
        QSettings s(CONFIG_FILE_NAME, QSettings::IniFormat);
        s.beginGroup(configGroup_);
        s.setValue(KEY_REMOTE_IP, remoteIp_->text());
        s.setValue(KEY_REMOTE_PORT, remotePort_->text());
        s.endGroup();
    }
}
