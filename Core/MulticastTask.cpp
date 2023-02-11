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

void MulticastTask::doStart(io_context &io)
{
    if (cfg_.localPort == 0 || cfg_.localPort > 0xFFFF)
    {
        throw std::logic_error(std::format("�˿�{}����", cfg_.localPort));
    }
    if (cfg_.multicastIp.isEmpty())
    {
        throw std::logic_error(std::string("�鲥��ַ����Ϊ��"));
    }
    /// todo ����鲥IP��ַ
    group_ = QString("%1:%2").arg(cfg_.remoteIp).arg(cfg_.remotePort);
    groupEndpoint_ = udp::endpoint(address::from_string(cfg_.multicastIp.toStdString()), cfg_.localPort);

    udp::endpoint listen(address::from_string(cfg_.localIp.toStdString()), cfg_.localPort);
    recv_ = std::make_unique<udp::socket>(io);
    recv_->open(udp::v4());
    recv_->set_option(udp::socket::reuse_address(true));
    recv_->bind(listen);

    auto groupAddr = make_address((cfg_.multicastIp.toStdString()));
    recv_->set_option(multicast::join_group(groupAddr));

    send_ = std::make_unique<udp::socket>(io, udp::v4());

    doRead();

    emit workStateChanged(OK);
}

void MulticastTask::doStop()
{
}

void MulticastTask::send(const QByteArray &data)
{
    sendBuffer_.resize(data.size());
    std::copy(data.begin(), data.end(), sendBuffer_.begin());
    recv_->async_send_to(buffer(sendBuffer_), groupEndpoint_, [this](auto &&ec, auto &&len) {
        if (ec)
        {
            emit logMessage(QString("�鲥�������ݵ�%1ʧ��").arg(group_));
        }
        else
        {
            emit logMessage(QString("�鲥����%1�ֽڵ�%2").arg(len).arg(group_));
        }
    });
}

void MulticastTask::doRead()
{
    recvBuffer_.resize(4096);
    recv_->async_receive(buffer(recvBuffer_), [this](auto &&ec, auto &&len) {
        if (ec)
        {
            if (recv_->is_open())
            {
                recv_->close();
            }
        }
        else
        {
            emit dataReceived(group_, QByteArray(recvBuffer_.data(), len));
            doRead();
        }
    });
}
