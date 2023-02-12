#pragma once

#include <QWidget>

class QLineEdit;
struct NetworkConfig;

class MulticastConfigWidget : public QWidget
{
    Q_OBJECT
public:
    MulticastConfigWidget(const QString &configGroup, QWidget *parent = nullptr);

public slots:
    void updateConfig(NetworkConfig &nc);

private:
    QString configGroup_;
    QLineEdit *multicastAddr_;
};
