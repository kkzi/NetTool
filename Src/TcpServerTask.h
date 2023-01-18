#pragma once

#include "NetworkTask.h"
#include <atomic>
#include <boost/asio/ip/tcp.hpp>
#include <memory>
#include <thread>

struct TcpSession;

class TcpServerTask : public NetworkTask
{
public:
    using NetworkTask::NetworkTask;
    ~TcpServerTask();

public:
    void start(boost::asio::io_context &io) override;
    void send(const QByteArray &) override;

private:
    void startSession(boost::asio::ip::tcp::socket &&sock);
    void doAccept();

private:
    std::unique_ptr<boost::asio::ip::tcp::acceptor> acc_{ nullptr };
    std::list<std::shared_ptr<TcpSession>> sessions_;
};
