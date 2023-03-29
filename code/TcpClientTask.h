#pragma once

#include "NetworkTask.h"
#include <boost/asio/ip/tcp.hpp>
#include <memory>
#include <string>
#include <vector>

class TcpClientTask : public NetworkTask
{
public:
    using NetworkTask::NetworkTask;
    ~TcpClientTask();

public:
    void send(const std::vector<std::byte> &) override;

protected:
    void do_start() override;
    void do_stop() override;

private:
    void do_read();

private:
    bool stopped_{ false };
    std::unique_ptr<boost::asio::ip::tcp::socket> sock_{ nullptr };
    std::string peer_;
    std::vector<std::byte> buffer_;
};
