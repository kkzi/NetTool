#include "TcpClientTask.h"
#include "socket_timeout_option.h"
#include <QEventLoop>
#include <boost/asio/write.hpp>

using namespace boost::asio;
using namespace boost::asio::ip;

TcpClientTask::~TcpClientTask()
{
}

void TcpClientTask::doStart(io_context &io)
{
    peer_ = QString("%1:%2").arg(cfg_.remoteIp).arg(cfg_.remotePort);

    tcp::endpoint local(address::from_string(cfg_.localIp.toStdString()), cfg_.localPort);
    tcp::endpoint remote(address::from_string(cfg_.remoteIp.toStdString()), cfg_.remotePort);
    sock_ = std::make_unique<tcp::socket>(io);
    sock_->open(tcp::v4());
    // sock_->bind(tcp::endpoint(tcp::v4(), 0));
    sock_->bind(local);
    sock_->set_option(send_timeout_option(3));
    auto addr = QString("%1:%2").arg(cfg_.remoteIp).arg(cfg_.remotePort);
    sock_->async_connect(remote, [this, addr](auto &&ec) {
        if (stopped_)
        {
            return;
        }
        if (ec)
        {
            emit logMessage(QString("连接TCP服务器%1失败").arg(addr));
            sock_->close();
        }
        else
        {
            emit logMessage(QString("连接TCP服务器%1成功").arg(addr));
            doRead();
        }
        emit workStateChanged(ec ? NetworkTask::FAILED : NetworkTask::OK);
    });
}

void TcpClientTask::doStop()
{
    stopped_ = true;
    if (sock_ != nullptr)
    {
        sock_->close();
        sock_.reset();
    }
}

void TcpClientTask::send(const QByteArray &data)
{
    Q_ASSERT(sock_ != nullptr);
    if (!sock_->is_open())
    {
        emit workStateChanged(FAILED);
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
    buffer_.resize(4096, 0);
    sock_->async_read_some(buffer(buffer_), [this](auto &&ec, auto &&len) {
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
            emit dataReceived(peer_, QByteArray(buffer_.data(), len));
            doRead();
        }
    });
}
