#pragma once

#include "NetworkConfig.h"
#include <QObject>

class NetworkTask : public QObject
{
    Q_OBJECT

public:
    virtual ~NetworkTask() {};

public:
    void setConfig(const NetworkConfig& cfg)
    {
        cfg_ = cfg;
    }

public:
    virtual void start() = 0;
    virtual void send(const QByteArray&) = 0;

signals:
    void dataReceived(const QByteArray&);

private:
    NetworkConfig cfg_;
};

