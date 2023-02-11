#pragma once

#include "NetworkTask.h"
#include <boost/asio/ip/tcp.hpp>
#include <memory>
#include <string>

class TcpClientTask : public NetworkTask
{
    Q_OBJECT
public:
    using NetworkTask::NetworkTask;
    ~TcpClientTask();

public:
    void doStart(io_context &io) override;
    void doStop() override;
    void send(const QByteArray &) override;

private:
    void doRead();

private:
    bool stopped_{ false };
    QString peer_;
    std::unique_ptr<boost::asio::ip::tcp::socket> sock_{ nullptr };
    std::string buffer_;
};
