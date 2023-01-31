#pragma once

#include "NetworkTask.h"
#include <boost/asio/ip/udp.hpp>
#include <memory>

class UdpTask : public NetworkTask
{
public:
    ~UdpTask();

public:
    Result start(io_context &io) override;
    void send(const QByteArray &) override;

private:
    void doRead();

private:
    std::unique_ptr<boost::asio::ip::udp::socket> sock_{ nullptr };
    std::string buffer_;
    QString peer_;
};
