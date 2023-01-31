#pragma once

#include "NetworkTask.h"
#include <QStringList>
#include <atomic>
#include <boost/asio/ip/tcp.hpp>
#include <list>
#include <memory>
#include <thread>

struct TcpSession;

class TcpServerTask : public NetworkTask, std::enable_shared_from_this<TcpServerTask>
{
    Q_OBJECT;

public:
    using NetworkTask::NetworkTask;
    ~TcpServerTask();

public:
    Result start(boost::asio::io_context &io) override;
    void send(const QByteArray &) override;

private:
    void startSession(boost::asio::ip::tcp::socket &&sock);
    void doAccept();
    QStringList clients() const;

signals:
    void sessionChanged(QStringList);

private:
    std::unique_ptr<boost::asio::ip::tcp::acceptor> acc_{ nullptr };
    std::list<std::shared_ptr<TcpSession>> sessions_;
};
