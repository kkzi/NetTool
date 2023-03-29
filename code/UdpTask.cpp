#include "UdpTask.h"
#include <boost/asio/write.hpp>

using namespace boost::asio;
using namespace boost::asio::ip;

UdpTask::~UdpTask()
{
    stop();
}

void UdpTask::do_start()
{
    stopped_ = false;
    peer_ = std::format("{}:{}", cfg_.remote_ip, cfg_.remote_port);
    remote_ = udp::endpoint(address::from_string(cfg_.remote_ip), cfg_.remote_port);

    udp::endpoint local(address::from_string(cfg_.local_ip), cfg_.local_port);
    sock_ = std::make_unique<udp::socket>(io_);
    sock_->open(udp::v4());
    sock_->bind(local);

    on_state_changed_(NetworkTask::WorkState::OK);

    do_read();
}

void UdpTask::do_stop()
{
    if (stopped_)
    {
        return;
    }
    if (sock_ != nullptr && sock_->is_open())
    {
        stopped_ = true;
        boost::system::error_code ec;
        sock_->close(ec);
    }
}

void UdpTask::send(const std::vector<std::byte> &data)
{
    // buffer_.resize(data.size());
    // std::copy(data.begin(), data.end(), buffer_.begin());
    auto bytes = sock_->send_to(buffer(data), remote_);
    on_message_(std::format("UDP send {} bytes to {}", bytes, peer_));
    //, [this](auto &&ec, auto &&len) {
    //    if (stopped_)
    //    {
    //        return;
    //    }
    //    if (ec)
    //    {
    //        emit logMessage(tr("UDP send data to %1 failed, detail=%2").arg(peer_).arg(ec.what().c_str()));
    //    }
    //    else
    //    {
    //        emit logMessage(tr("UDP send %1 bytes to %2").arg(len).arg(peer_));
    //    }
    //});
}

void UdpTask::do_read()
{
    buffer_.resize(4096);
    sock_->async_receive(buffer(buffer_), [this](auto &&ec, auto &&len) {
        if (stopped_)
        {
            return;
        }
        if (!ec)
        {
            buffer_.resize(len);
            on_data_(peer_, buffer_);
            do_read();
        }
    });
}
