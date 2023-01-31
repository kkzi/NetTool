#pragma once

#include <QWidget>

class QLineEdit;
struct NetworkConfig;

class MulticastConfigWidget : public QWidget
{
    Q_OBJECT
public:
    MulticastConfigWidget(QWidget *parent = nullptr);

public slots:
    void updateConfig(NetworkConfig &nc);

private:
    QLineEdit *multicastAddr_;
};
