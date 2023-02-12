#pragma once

#include <QHBoxLayout>
#include <QLabel>

static QHBoxLayout *makeLabeledField(const QString &title, QWidget *field, int labelWidth, int fieldWidth)
{
    auto label = new QLabel(title);
    label->setObjectName("label");
    // label->setFixedWidth(72);
    label->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    if (labelWidth > 0)
    {
        label->setFixedWidth(labelWidth);
    }
    if (fieldWidth > 0)
    {
        field->setFixedWidth(fieldWidth);
    }
    if (field->objectName().isEmpty())
    {
        field->setObjectName("editor");
    }
    auto row = new QHBoxLayout;
    row->addWidget(label);
    row->addWidget(field, 1);
    return row;
}

static QHBoxLayout *makeLabeledField(const QString &title, QWidget *field, int fieldWidth = 120)
{
    return makeLabeledField(title, field, 0, fieldWidth);
}
