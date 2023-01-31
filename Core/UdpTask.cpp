#include "UdpTask.h"
#include <boost/asio/write.hpp>

using namespace boost::asio;
using namespace boost::asio::ip;

UdpTask::~UdpTask()
{
    if (sock_ != nullptr && sock_->is_open())
    {
        sock_->close();
    }
}

Result UdpTask::start(io_context &io)
{
    buffer_.resize(4096, 0);
    peer_ = QString("%1:%2").arg(cfg_.remoteIp).arg(cfg_.remotePort);

    udp::endpoint local(address::from_string(cfg_.localIp.toStdString()), cfg_.localPort);
    udp::endpoint remote(address::from_string(cfg_.remoteIp.toStdString()), cfg_.remotePort);
    sock_ = std::make_unique<udp::socket>(io);
    sock_->open(udp::v4());
    sock_->bind(local);

    doRead();
    return std::make_tuple(true, "");
}

void UdpTask::send(const QByteArray &data)
{
    buffer_.resize(data.size());
    std::copy(data.begin(), data.end(), buffer_.begin());
    sock_->async_send(buffer(buffer_), [this](auto &&ec, auto &&len) {
        if (ec)
        {
            emit logMessage(QString("UDP发送数据到%1失败").arg(peer_));
        }
        else
        {
            emit logMessage(QString("UDP发送%1字节到%2").arg(len).arg(peer_));
        }
    });
}

void UdpTask::doRead()
{
    buffer_.resize(4096);
    sock_->async_receive(buffer(buffer_), [this](auto &&ec, auto &&len) {
        if (ec)
        {
            sock_->close();
        }
        else
        {
            emit dataReceived(peer_, QByteArray(buffer_.data(), len));
            doRead();
        }
    });
}
