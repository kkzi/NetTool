#pragma once


#include <QLabel>
#include <QHBoxLayout>


static QHBoxLayout* makeFormRow(QString title, QWidget* widget)
{
    auto label = new QLabel(title);
    label->setObjectName("label");
    label->setFixedWidth(60);
    if (widget->objectName().isEmpty())
    {
        widget->setObjectName("editor");
    }
    auto row = new QHBoxLayout;
    row->addWidget(label);
    row->addWidget(widget, 1);
    return row;
}

