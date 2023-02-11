#pragma once

#include <QHBoxLayout>
#include <QLabel>

static QHBoxLayout *makeLabeledField(QString title, QWidget *field, int fieldWidth = 120)
{
    auto label = new QLabel(title);
    label->setObjectName("label");
    // label->setFixedWidth(72);
    label->setAlignment(Qt::AlignRight | Qt::AlignVCenter);

    if (field->objectName().isEmpty())
    {
        field->setObjectName("editor");
    }
    if (fieldWidth > 0)
    {
        field->setFixedWidth(fieldWidth);
    }
    auto row = new QHBoxLayout;
    row->addWidget(label);
    row->addWidget(field, 1);
    return row;
}
