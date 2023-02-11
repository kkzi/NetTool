#include "UdpTask.h"
#include <boost/asio/write.hpp>

using namespace boost::asio;
using namespace boost::asio::ip;

UdpTask::~UdpTask()
{
}

void UdpTask::doStart(io_context &io)
{
    stopped_ = false;
    peer_ = QString("%1:%2").arg(cfg_.remoteIp).arg(cfg_.remotePort);
    remote_ = udp::endpoint(address::from_string(cfg_.remoteIp.toStdString()), cfg_.remotePort);

    udp::endpoint local(address::from_string(cfg_.localIp.toStdString()), cfg_.localPort);
    sock_ = std::make_unique<udp::socket>(io);
    sock_->open(udp::v4());
    sock_->bind(local);

    emit workStateChanged(OK);

    doRead();
}

void UdpTask::doStop()
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

void UdpTask::send(const QByteArray &data)
{
    buffer_.resize(data.size());
    std::copy(data.begin(), data.end(), buffer_.begin());
    auto bytes = sock_->send_to(buffer(buffer_, buffer_.size()), remote_);
    emit logMessage(tr("UDP send %1 bytes to %2").arg(bytes).arg(peer_));
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

void UdpTask::doRead()
{
    buffer_.resize(4096);
    sock_->async_receive(buffer(buffer_), [this](auto &&ec, auto &&len) {
        if (stopped_)
        {
            return;
        }
        if (!ec)
        {
            emit dataReceived(peer_, QByteArray(buffer_.data(), len));
            doRead();
        }
    });
}
