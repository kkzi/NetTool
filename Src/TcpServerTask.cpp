#include "TcpServerTask.h"
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/read.hpp>
#include <boost/asio/write.hpp>
#include <deque>
#include <format>
#include <functional>
#include <string>

using namespace boost::asio::ip;

struct TcpSession : public std::enable_shared_from_this<TcpSession>
{
    TcpSession(tcp::socket sock)
        : sock_(std::move(sock))
    {
    }

    operator bool() const
    {
        return available_;
    }

    void start(std::function<void(char *, std::size_t len)> reader)
    {
        doRead(std::move(reader));
    }

    void send(const char *data, size_t len)
    {
        bool inProcess = !queue_.empty();
        queue_.emplace_back(data, len);
        if (!inProcess)
        {
            doWrite();
        }
    }

private:
    void doRead(std::function<void(char *, std::size_t len)> reader)
    {
        boost::asio::async_read(sock_, buffer(buffer_), [reader, this](auto &&ec, auto &&len) {
            if (ec)
            {
                available_ = false;
            }
            else
            {
                reader(buffer_, len);
                doRead(reader);
            }
        });
    }

    void doWrite()
    {
        auto self(shared_from_this());
        boost::asio::async_write(sock_, boost::asio::buffer(queue_.front().data(), queue_.front().length()),
            [this, self](boost::system::error_code ec, std::size_t) {
                if (ec)
                {
                    available_ = false;
                }
                else
                {
                    queue_.pop_front();
                    if (!queue_.empty())
                    {
                        doWrite();
                    }
                }
            });
    }

private:
    tcp::socket sock_;
    char buffer_[4096]{ 0 };
    std::deque<std::string> queue_;
    bool available_{ true };
};

/************************************************************************/
/*                                                                      */
/************************************************************************/
TcpServerTask::~TcpServerTask()
{
    sessions_.clear();
    acc_->cancel();
    acc_->close();
}

void TcpServerTask::start(boost::asio::io_context &io)
{
    Q_ASSERT(!cfg_.localIp.isEmpty());
    Q_ASSERT(cfg_.localPort > 0);

    tcp::endpoint local(address::from_string(cfg_.localIp.toStdString()), cfg_.localPort);
    acc_ = std::make_unique<tcp::acceptor>(io, local);
    doAccept();
}

void TcpServerTask::send(const QByteArray &data)
{
    for (auto &s : sessions_)
    {
        if (s)
        {
            s->send(data.data(), data.length());
        }
        else
        {
            sessions_.remove(s);
        }
    }
}

void TcpServerTask::startSession(boost::asio::ip::tcp::socket &&sock)
{
    auto session = std::make_shared<TcpSession>(std::move(sock));
    sessions_.emplace_back(session);
    session->start([this](auto &&ptr, auto &&len) {
        QByteArray ba((const char *)ptr, len);
        emit dataReceived(ba);
    });
}

void TcpServerTask::doAccept()
{
    acc_->async_accept([this](auto &&ec, auto &&sock) {
        if (!ec)
        {
            startSession(std::move(sock));
            doAccept();
        }
    });
}
