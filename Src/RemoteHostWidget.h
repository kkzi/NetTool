#pragma once


#include <QWidget>

class QLineEdit;
struct NetworkConfig;

class RemoteHostWidget :public QWidget
{
    Q_OBJECT
public:
    RemoteHostWidget(QWidget* parent = nullptr);

public slots:
    void updateConfig(NetworkConfig& nc);

private:
    QLineEdit* remoteIp_;
    QLineEdit* remotePort_;
};

