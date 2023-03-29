#include "MulticastTask.h"
#include <boost/asio/basic_socket.hpp>
#include <boost/asio/ip/multicast.hpp>

using namespace boost::asio;
using namespace boost::asio::ip;

MulticastTask::~MulticastTask()
{
    // if (send_ != nullptr && send_->is_open())
    //{
    //    send_->close();
    //}
    // if (recv_ != nullptr && recv_->is_open())
    //{
    //    recv_->close();
    //}
    // buffer_.clear();
}

void MulticastTask::do_start()
{
    if (cfg_.local_port == 0 || cfg_.local_port > 0xFFFF)
    {
        on_message_(std::format("Invalid port {}", cfg_.local_port));
        return;
    }
    if (cfg_.remote_ip.empty())
    {
        on_message_(std::format("Invalid multicast ip address {}", cfg_.remote_ip));
        return;
    }
    /// todo ¼ì²é×é²¥IPµØÖ·
    group_ = std::format("{}:{}", cfg_.remote_ip, cfg_.local_port);
    group_ep_ = udp::endpoint(address::from_string(cfg_.remote_ip), cfg_.local_port);

    udp::endpoint listen(address::from_string(cfg_.local_ip), cfg_.local_port);
    recv_ = std::make_unique<udp::socket>(io_);
    recv_->open(udp::v4());
    recv_->set_option(udp::socket::reuse_address(true));
    recv_->bind(listen);

    auto group_addr = make_address((cfg_.remote_ip));
    recv_->set_option(multicast::join_group(group_addr));

    send_ = std::make_unique<udp::socket>(io_, udp::v4());

    do_read();
    on_state_changed_(NetworkTask::WorkState::OK);
}

void MulticastTask::do_stop()
{
}

void MulticastTask::send(const std::vector<std::byte> &data)
{
    send_buffer_.resize(data.size());
    std::copy(data.begin(), data.end(), send_buffer_.begin());
    recv_->async_send_to(buffer(send_buffer_), group_ep_, [this](auto &&ec, auto &&len) {
        if (ec)
        {
            on_message_(std::format("Send multicast message to {} failed", group_));
        }
        else
        {
            on_message_(std::format("Send {} bytes to multicast {}", len, group_));
        }
    });
}

void MulticastTask::do_read()
{
    recv_buffer_.resize(4096);
    recv_->async_receive(buffer(recv_buffer_), [this](auto &&ec, auto &&len) {
        if (ec)
        {
            if (recv_->is_open())
            {
                recv_->close();
            }
        }
        else
        {
            recv_buffer_.resize(len);
            on_data_(group_, recv_buffer_);
            do_read();
        }
    });
}
