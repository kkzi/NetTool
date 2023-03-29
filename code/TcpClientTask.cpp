#include "TcpClientTask.h"
#include "socket_timeout_option.h"
#include <boost/asio/write.hpp>
#include <format>

using namespace boost::asio;
using namespace boost::asio::ip;

TcpClientTask::~TcpClientTask()
{
    stop();
}

void TcpClientTask::do_start()
{
    peer_ = std::format("{}:{}", cfg_.remote_ip, cfg_.remote_port);

    tcp::endpoint local(address::from_string(cfg_.local_ip), cfg_.local_port);
    tcp::endpoint remote(address::from_string(cfg_.remote_ip), cfg_.remote_port);
    sock_ = std::make_unique<tcp::socket>(io_);
    sock_->open(tcp::v4());
    // sock_->bind(tcp::endpoint(tcp::v4(), 0));
    sock_->bind(local);
    sock_->set_option(send_timeout_option(3));
    auto addr = std::format("{}:{}", cfg_.remote_ip, cfg_.remote_port);
    sock_->async_connect(remote, [this, addr](auto &&ec) {
        if (stopped_)
        {
            return;
        }
        if (ec)
        {
            on_message_(std::format("Connect to TCP server {} failed", addr));
            sock_->close();
        }
        else
        {
            on_message_(std::format("Connect to TCP server {} successed", addr));
            do_read();
        }
        on_state_changed_(ec ? NetworkTask::WorkState::FAILED : NetworkTask::WorkState::OK);
    });
}

void TcpClientTask::do_stop()
{
    stopped_ = true;
    if (sock_ != nullptr)
    {
        sock_->close();
        sock_.reset();
    }
}

void TcpClientTask::send(const std::vector<std::byte> &data)
{
    assert(sock_ != nullptr);
    if (!sock_->is_open())
    {
        on_state_changed_(NetworkTask::WorkState::FAILED);
        return;
    }
    buffer_.resize(data.size());
    std::copy(data.begin(), data.end(), buffer_.begin());
    boost::asio::async_write(*sock_, buffer(buffer_), [this](auto &&ec, auto &&len) {
        if (stopped_)
        {
            return;
        }
        if (ec)
        {
            on_message_(std::format("Send message to TCP server {} failed", peer_));
        }
        else
        {
            on_message_(std::format("Send {} bytes to TCP server {}", len, peer_));
        }
    });
}

void TcpClientTask::do_read()
{
    buffer_.resize(4096);
    sock_->async_read_some(boost::asio::buffer(buffer_), [this](auto &&ec, auto &&len) {
        if (stopped_)
        {
            return;
        }

        if (ec)
        {
            sock_->close();
        }
        else
        {
            buffer_.resize(len);
            on_data_(peer_, buffer_);
            do_read();
        }
    });
}
