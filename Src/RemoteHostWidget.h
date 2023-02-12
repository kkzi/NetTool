#pragma once

#include <QWidget>

class QLineEdit;
struct NetworkConfig;

class RemoteHostWidget : public QWidget
{
    Q_OBJECT
public:
    RemoteHostWidget(const QString &configGroup, QWidget *parent = nullptr);

public slots:
    void updateConfig(NetworkConfig &nc);

private:
    QString configGroup_;
    QLineEdit *remoteIp_;
    QLineEdit *remotePort_;
};
