#pragma once

#include "NetworkTask.h"
#include <boost/asio/ip/udp.hpp>
#include <memory>

class UdpTask : public NetworkTask
{
public:
    using NetworkTask::NetworkTask;
    ~UdpTask();

public:
    void send(const std::vector<std::byte> &) override;

protected:
    void do_start() override;
    void do_stop() override;

private:
    void do_read();

private:
    std::unique_ptr<boost::asio::ip::udp::socket> sock_{ nullptr };
    std::vector<std::byte> buffer_;
    bool stopped_{ false };
    std::string peer_;
    boost::asio::ip::udp::endpoint remote_;
};
