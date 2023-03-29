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
        addr_ = std::format("{}:{}", ep.address().to_string(), ep.port());
    }

    ~TcpSession()
    {
        stopped_ = true;
        boost::system::error_code ec;
        sock_.close(ec);
    }

    void start(std::function<void(const std::vector<std::byte> &)> reader)
    {
        do_read(std::move(reader));
    }

    void send(const char *data, size_t len)
    {
        bool in_process = !queue_.empty();
        queue_.emplace_back(data, len);
        if (!in_process)
        {
            do_write();
        }
    }

    std::string peer_address() const
    {
        return addr_;
    }

private:
    void do_read(std::function<void(const std::vector<std::byte> &)> reader)
    {
        auto self(shared_from_this());
        buffer_.resize(4095);
        sock_.async_read_some(boost::asio::buffer(buffer_, buffer_.size()), [reader, self, this](auto &&ec, auto &&len) {
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
                buffer_.resize(len);
                reader(buffer_);
                self->do_read(std::move(reader));
            }
        });
    }

    void do_write()
    {
        auto self(shared_from_this());
        boost::asio::async_write(
            sock_, boost::asio::buffer(queue_.front().data(), queue_.front().length()), [this, self](boost::system::error_code ec, std::size_t) {
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
                        do_write();
                    }
                }
            });
    }

private:
    tcp::socket sock_;
    bool stopped_{ false };
    std::string addr_;
    std::function<void(std::shared_ptr<TcpSession>)> drop_;
    std::vector<std::byte> buffer_;
    std::deque<std::string> queue_;
};

/************************************************************************/
/*                                                                      */
/************************************************************************/
TcpServerTask::~TcpServerTask()
{
    stop();
}

void TcpServerTask::do_start()
{
    assert(!cfg_.local_ip.empty());
    assert(cfg_.local_port > 0);

    tcp::endpoint local(address::from_string(cfg_.local_ip), cfg_.local_port);
    acc_ = std::make_unique<tcp::acceptor>(io_, local);
    do_accept();

    on_message_(std::format("A TCP server listening on {}:{} is running", cfg_.local_ip, cfg_.local_port));
    on_state_changed_(WorkState::OK);
}

void TcpServerTask::do_stop()
{
    if (!stopped_)
    {
        stopped_ = true;
        acc_->cancel();
        acc_->close();
        sessions_.clear();
    }
}

void TcpServerTask::send(const std::vector<std::byte> &data)
{
    for (auto &s : sessions_)
    {
        if (s)
        {
            s->send((char *)data.data(), data.size());
        }
    }
}

std::vector<std::string> TcpServerTask::clients() const
{
    std::vector<std::string> arr;
    for (auto &&s : sessions_)
    {
        arr.push_back(s->peer_address());
    }
    return arr;
}

void TcpServerTask::start_session(boost::asio::ip::tcp::socket &&sock)
{
    auto session = std::make_shared<TcpSession>(std::move(sock), [this](auto &&s) {
        if (stopped_)
        {
            return;
        }
        std::erase(sessions_, s);
        auto addr = this->clients();
        on_message_(std::format("TCP connection disconnected from {}", s->peer_address()));
        s.reset();
    });
    auto from = session->peer_address();
    on_message_(std::format("TCP connection established from {}", from));
    sessions_.emplace_back(session);
    session->start([this, from](auto &&data) {
        if (stopped_)
        {
            return;
        }
        if (data.empty())
        {
            return;
        }
        on_data_(from, data);
    });
}

void TcpServerTask::do_accept()
{
    acc_->async_accept([this](auto &&ec, auto &&sock) {
        if (stopped_)
        {
            return;
        }
        if (!ec)
        {
            start_session(std::move(sock));
            do_accept();
        }
    });
}
