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
    TcpSession(tcp::socket sock, std::function<void(std::shared_ptr<TcpSession>)> drop)
        : sock_(std::move(sock))
        , drop_(drop)
    {
        auto ep = sock_.remote_endpoint();
        addr_ = QString("%1:%2").arg(ep.address().to_string().c_str()).arg(ep.port());
    }

    ~TcpSession()
    {
        stopped_ = true;
        boost::system::error_code ec;
        sock_.close(ec);
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

    QString remoteAddress() const
    {
        return addr_;
    }

private:
    void doRead(std::function<void(char *, std::size_t len)> reader)
    {
        auto self(shared_from_this());
        buffer_.resize(4095);
        sock_.async_read_some(buffer(buffer_, buffer_.size()), [reader, self, this](auto &&ec, auto &&len) {
            if (stopped_)
            {
                return;
            }
            if (ec)
            {
                drop_(self);
            }
            else
            {
                reader(buffer_.data(), len);
                self->doRead(reader);
            }
        });
    }

    void doWrite()
    {
        auto self(shared_from_this());
        boost::asio::async_write(sock_, boost::asio::buffer(queue_.front().data(), queue_.front().length()),
            [this, self](boost::system::error_code ec, std::size_t) {
                if (stopped_)
                {
                    return;
                }
                if (ec)
                {
                    drop_(self);
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
    bool stopped_{ false };
    QString addr_;
    std::function<void(std::shared_ptr<TcpSession>)> drop_;
    std::string buffer_;
    std::deque<std::string> queue_;
};

/************************************************************************/
/*                                                                      */
/************************************************************************/
TcpServerTask::~TcpServerTask()
{
}

void TcpServerTask::doStart(boost::asio::io_context &io)
{
    Q_ASSERT(!cfg_.localIp.isEmpty());
    Q_ASSERT(cfg_.localPort > 0);

    tcp::endpoint local(address::from_string(cfg_.localIp.toStdString()), cfg_.localPort);
    acc_ = std::make_unique<tcp::acceptor>(io, local);
    doAccept();

    emit logMessage(tr("A TCP server listening on %1:%2 is running").arg(cfg_.localIp).arg(cfg_.localPort));
    emit workStateChanged(OK);
}

void TcpServerTask::doStop()
{
    stopped_ = true;
    acc_->cancel();
    acc_->close();
    sessions_.clear();
}

void TcpServerTask::send(const QByteArray &data)
{
    for (auto &s : sessions_)
    {
        if (s)
        {
            s->send(data.data(), data.length());
        }
    }
}

QStringList TcpServerTask::clients() const
{
    QStringList arr;
    for (auto &&s : sessions_)
    {
        arr << s->remoteAddress();
    }
    return arr;
}

void TcpServerTask::startSession(boost::asio::ip::tcp::socket &&sock)
{
    auto session = std::make_shared<TcpSession>(std::move(sock), [this](auto &&s) {
        if (stopped_)
        {
            return;
        }
        std::erase(sessions_, s);
        auto addr = this->clients();
        emit logMessage(tr("TCP connection disconnected from %1").arg(s->remoteAddress()));
        emit sessionChanged(addr);
        s.reset();
    });
    auto from = session->remoteAddress();
    emit logMessage(tr("TCP connection established from %1").arg(from));
    sessions_.emplace_back(session);
    session->start([this, from](auto &&ptr, auto &&len) {
        if (stopped_)
        {
            return;
        }
        if (len == 0)
        {
            return;
        }
        QByteArray ba((const char *)ptr, len);
        emit dataReceived(from, ba);
    });
    emit sessionChanged(clients());
}

void TcpServerTask::doAccept()
{
    acc_->async_accept([this](auto &&ec, auto &&sock) {
        if (stopped_)
        {
            return;
        }
        if (!ec)
        {
            startSession(std::move(sock));
            doAccept();
        }
    });
}
