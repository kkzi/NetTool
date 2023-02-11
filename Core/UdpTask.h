#pragma once

#include "NetworkTask.h"
#include <boost/asio/ip/udp.hpp>
#include <memory>

class UdpTask : public NetworkTask
{
public:
    ~UdpTask();

public:
    void send(const QByteArray &) override;

protected:
    void doStart(io_context &io) override;
    void doStop() override;

private:
    void doRead();

private:
    std::unique_ptr<boost::asio::ip::udp::socket> sock_{ nullptr };
    std::string buffer_;
    bool stopped_{ false };
    QString peer_;
    boost::asio::ip::udp::endpoint remote_;
};
