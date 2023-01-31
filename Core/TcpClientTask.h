#pragma once

#include "NetworkTask.h"
#include <boost/asio/ip/tcp.hpp>
#include <memory>
#include <string>

class TcpClientTask : public NetworkTask
{
public:
    using NetworkTask::NetworkTask;
    ~TcpClientTask();

public:
    Result start(io_context &io) override;
    void send(const QByteArray &) override;

private:
    void doRead();

private:
    QString peer_;
    std::unique_ptr<boost::asio::ip::tcp::socket> sock_{ nullptr };
    std::string buffer_;
};
