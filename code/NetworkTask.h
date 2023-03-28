#pragma once

#include "NetworkConfig.h"
#include <boost/asio/io_context.hpp>
#include <functional>
#include <string_view>
#include <thread>

class NetworkTask
{
public:
    enum class WorkState : int
    {
        OK,
        FAILED,
    };

public:
    NetworkTask(NetworkConfig cfg);
    virtual ~NetworkTask();

public:
    NetworkTask &on_state_changed(std::function<void(WorkState)>);
    NetworkTask &on_message(std::function<void(std::string_view)>);
    NetworkTask &on_data(std::function<void(std::string_view from, const std::vector<std::byte> &data)>);
    void start();
    void stop();

public:
    virtual void send(const std::vector<std::byte> &) = 0;

protected:
    virtual void do_start() = 0;
    virtual void do_stop() = 0;

protected:
    boost::asio::io_context io_;
    NetworkConfig cfg_;
    std::function<void(WorkState)> on_state_changed_;
    std::function<void(std::string_view)> on_message_;
    std::function<void(std::string_view from, const std::vector<std::byte> &)> on_data_;
    std::thread thread_;
};
