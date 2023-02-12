#pragma once

#include <QWidget>

class PlaceholderWidget : public QWidget
{
public:
    PlaceholderWidget(QString, QWidget *parent = nullptr)
        : QWidget(parent)
    {
    }
};
