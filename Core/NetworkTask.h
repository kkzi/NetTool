#pragma once

#include "NetworkConfig.h"
#include "Result.h"
#include <QObject>
#include <boost/asio/io_context.hpp>

using namespace boost::asio;

class NetworkTask : public QObject
{
    Q_OBJECT

public:
    enum WorkState
    {
        OK,
        FAILED,
    };

public:
    using QObject::QObject;
    virtual ~NetworkTask();

public:
    void setConfig(const NetworkConfig &cfg)
    {
        cfg_ = cfg;
    }

    QString protocol() const
    {
        return cfg_.protocol;
    }

public:
    void start();
    void stop();

public:
    virtual void send(const QByteArray &) = 0;

protected:
    virtual void doStart(io_context &io) = 0;
    virtual void doStop() = 0;

signals:
    void workStateChanged(int);
    void logMessage(const QString &);
    void dataReceived(const QString &from, const QByteArray &);

protected:
    boost::asio::io_context io_;
    std::thread thread_;
    NetworkConfig cfg_;
};
