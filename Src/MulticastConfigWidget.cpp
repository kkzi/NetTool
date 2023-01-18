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
    auto layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addLayout(makeFormRow("×é²¥µØÖ·", multicastAddr_));
    layout->addStretch();
}

void MulticastConfigWidget::updateConfig(NetworkConfig &nc)
{
    nc.multicastIp = multicastAddr_->text();
}

