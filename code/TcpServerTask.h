#pragma once

#include "NetworkTask.h"
#include <atomic>
#include <boost/asio/ip/tcp.hpp>
#include <list>
#include <memory>
#include <thread>

struct TcpSession;

class TcpServerTask : public NetworkTask, std::enable_shared_from_this<TcpServerTask>
{
public:
    using NetworkTask::NetworkTask;
    ~TcpServerTask() override;

public:
    void do_start() override;
    void do_stop() override;
    void send(const std::vector<std::byte> &) override;

private:
    void start_session(boost::asio::ip::tcp::socket &&sock);
    void do_accept();
    std::vector<std::string> clients() const;

private:
    std::atomic<bool> stopped_{ false };
    std::unique_ptr<boost::asio::ip::tcp::acceptor> acc_{ nullptr };
    std::list<std::shared_ptr<TcpSession>> sessions_;
};
