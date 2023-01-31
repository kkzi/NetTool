#pragma once

#include "Result.h"
#include "NetworkConfig.h"
#include <QObject>
#include <boost/asio/io_context.hpp>

using namespace boost::asio;

class NetworkTask : public QObject
{
    Q_OBJECT

public:
    using QObject::QObject;
    virtual ~NetworkTask(){};

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
    virtual Result start(io_context &io) = 0;
    virtual void send(const QByteArray &) = 0;

signals:
    void logMessage(const QString &);
    void dataReceived(const QString &from, const QByteArray &);

protected:
    NetworkConfig cfg_;
};
