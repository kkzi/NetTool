#pragma once

#include <QWidget>

class QLineEdit;
struct NetworkConfig;

class MulticastConfigWidget : public QWidget
{
public:
    MulticastConfigWidget(QWidget *parent = nullptr);

public:
    void updateConfig(NetworkConfig &nc);

private:
    QLineEdit *multicastAddr_;
};
