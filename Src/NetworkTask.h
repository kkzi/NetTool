#pragma once

#include "NetworkConfig.h"
#include <QObject>
#include <boost/asio/io_context.hpp>

using namespace boost::asio;

class NetworkTask : public QObject
{
    Q_OBJECT

public:
    virtual ~NetworkTask(){};

public:
    void setConfig(const NetworkConfig &cfg)
    {
        cfg_ = cfg;
    }

public:
    virtual void start(io_context &io) = 0;
    virtual void send(const QByteArray &) = 0;

signals:
    void dataReceived(const QByteArray &);

protected:
    NetworkConfig cfg_;
};
