#pragma once

#include "NetworkTask.h"
#include <boost/asio/ip/udp.hpp>
#include <memory>

class MulticastTask : public NetworkTask
{
public:
    ~MulticastTask();

public:
    Result start(io_context &io) override;
    void send(const QByteArray &) override;

private:
    void doRead();

private:
    std::unique_ptr<boost::asio::ip::udp::socket> recv_{ nullptr };
    std::unique_ptr<boost::asio::ip::udp::socket> send_{ nullptr };
    boost::asio::ip::udp::endpoint groupEndpoint_;
    QString group_;
    std::string recvBuffer_;
    std::string sendBuffer_;
};
