#pragma once

#include "NetworkTask.h"
#include <boost/asio/ip/udp.hpp>
#include <memory>
#include <vector>

class MulticastTask : public NetworkTask
{
public:
    using NetworkTask::NetworkTask;
    ~MulticastTask() override;

public:
    void send(const std::vector<std::byte> &) override;

protected:
    void do_start() override;
    void do_stop() override;

private:
    void do_read();

private:
    std::unique_ptr<boost::asio::ip::udp::socket> recv_{ nullptr };
    std::unique_ptr<boost::asio::ip::udp::socket> send_{ nullptr };
    boost::asio::ip::udp::endpoint group_ep_;
    std::string group_;
    std::vector<std::byte> recv_buffer_;
    std::vector<std::byte> send_buffer_;
};
