#include "TitledWidget.h"
#include <QLabel>
#include <QLayoutItem>
#include <QVBoxLayout>

TitledWidget::TitledWidget(const QString &title, QWidget *corner, QWidget *central, QWidget *parent /*= nullptr*/)
    : QFrame(parent)
    , corner_(corner)
    , central_(central)
{
    Q_CHECK_PTR(central);

    auto titleWrap = new QFrame(this);
    titleWrap->setObjectName("title");
    auto titleLayout = new QHBoxLayout(titleWrap);
    titleLayout->setContentsMargins(0, 0, 0, 0);
    titleLayout->addWidget(new QLabel(title, this), 1);
    if (corner != nullptr)
    {
        titleLayout->addStretch();
        titleLayout->addWidget(corner);
    }

    auto layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    layout->addWidget(titleWrap);
    layout->addWidget(central_, 1);
}

TitledWidget::~TitledWidget()
{
}
