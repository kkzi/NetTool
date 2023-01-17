#pragma once

#include <QWidget>

struct NetworkConfig;
class QLineEdit;

class MulticastConfigWidget : public QWidget
{
public:
    MulticastConfigWidget(QWidget *parent = nullptr);

public:
    void updateConfig(NetworkConfig &nc);

private:
    QLineEdit *multicastAddr_;
};
