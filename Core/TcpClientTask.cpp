#include "TcpClientTask.h"
#include <boost/asio/write.hpp>

using namespace boost::asio;
using namespace boost::asio::ip;

TcpClientTask::~TcpClientTask()
{
    if (sock_ != nullptr)
    {
        sock_->close();
        sock_.reset();
    }
}

Result TcpClientTask::start(io_context &io)
{
    buffer_.resize(4096, 0);
    peer_ = QString("%1:%2").arg(cfg_.remoteIp).arg(cfg_.remotePort);

    tcp::endpoint local(address::from_string(cfg_.localIp.toStdString()), cfg_.localPort);
    tcp::endpoint remote(address::from_string(cfg_.remoteIp.toStdString()), cfg_.remotePort);
    sock_ = std::make_unique<tcp::socket>(io);
    sock_->open(tcp::v4());
    // sock_->bind(tcp::endpoint(tcp::v4(), 0));
    sock_->bind(local);
    sock_->async_connect(remote, [this](auto &&ec) {
        if (ec)
        {
            emit logMessage(QString("连接TCP服务器%1:%2失败").arg(cfg_.remoteIp).arg(cfg_.remotePort));
            sock_->close();
        }
        else
        {
            emit logMessage(QString("连接TCP服务器%1:%2成功").arg(cfg_.remoteIp).arg(cfg_.remotePort));
            doRead();
        }
    });
    return std::make_tuple(true, "");
}

void TcpClientTask::send(const QByteArray &data)
{
    Q_ASSERT(sock_ != nullptr && sock_->is_open());
    buffer_.resize(data.size());
    std::copy(data.begin(), data.end(), buffer_.begin());
    boost::asio::async_write(*sock_, buffer(buffer_), [this](auto &&ec, auto &&len) {
        if (ec)
        {
            emit logMessage(QString("向TCP服务器%1发送数据失败").arg(peer_));
        }
        else
        {
            emit logMessage(QString("连接TCP服务器%1发送%2字节").arg(peer_).arg(len));
        }
    });
}

void TcpClientTask::doRead()
{
    sock_->async_read_some(buffer(buffer_), [this](auto &&ec, auto &&len) {
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
