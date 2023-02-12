#include "MulticastConfigWidget.h"
#include "CommonUi.h"
#include "NetworkConfig.h"
#include "NetworkTask.h"
#include <QLineEdit>
#include <QSettings>
#include <QVBoxLayout>
#include <QWidget>

MulticastConfigWidget::MulticastConfigWidget(const QString &configGroup, QWidget *parent /*= nullptr*/)
    : QWidget(parent)
    , configGroup_(configGroup)
    , multicastAddr_(new QLineEdit)
{
    {
        QSettings s(CONFIG_FILE_NAME, QSettings::IniFormat);
        auto multicastIp = s.value(QString("%1/%2").arg(configGroup_).arg(KEY_MULTICAST_IP)).toString();
        if (multicastIp.isEmpty())
        {
            multicastIp = "235.1.1.1";
        }
        multicastAddr_->setText(multicastIp);
    }

    auto layout = new QHBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addLayout(makeLabeledField(tr("Multicast IP"), multicastAddr_));
    layout->addStretch();
}

void MulticastConfigWidget::updateConfig(NetworkConfig &nc)
{
    nc.multicastIp = multicastAddr_->text();

    QSettings s(CONFIG_FILE_NAME, QSettings::IniFormat);
    s.beginGroup(configGroup_);
    s.setValue(KEY_MULTICAST_IP, multicastAddr_->text());
    s.endGroup();
}
