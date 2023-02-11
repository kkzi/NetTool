#include "MulticastConfigWidget.h"
#include "CommonUi.h"
#include "NetworkConfig.h"
#include "NetworkTask.h"
#include <QLineEdit>
#include <QVBoxLayout>
#include <QWidget>

MulticastConfigWidget::MulticastConfigWidget(QWidget *parent /*= nullptr*/)
    : QWidget(parent)
    , multicastAddr_(new QLineEdit)
{
    auto layout = new QHBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addLayout(makeLabeledField(tr("Multicast IP"), multicastAddr_));
    layout->addStretch();
}

void MulticastConfigWidget::updateConfig(NetworkConfig &nc)
{
    nc.multicastIp = multicastAddr_->text();
}

