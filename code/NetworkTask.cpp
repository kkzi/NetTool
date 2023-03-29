#include "NetworkTask.h"

NetworkTask::NetworkTask(NetworkConfig cfg)
    : cfg_(std::move(cfg))
{
}

NetworkTask::~NetworkTask()
{
}

NetworkTask &NetworkTask::on_state_changed(std::function<void(WorkState)> func)
{
    on_state_changed_ = std::move(func);
    return *this;
}

NetworkTask &NetworkTask::on_message(std::function<void(std::string_view)> func)
{
    on_message_ = std::move(func);
    return *this;
}

NetworkTask &NetworkTask::on_data(std::function<void(std::string_view, const std::vector<std::byte> &)> func)
{
    on_data_ = std::move(func);
    return *this;
}

void NetworkTask::start()
{
    do_start();
    thread_ = std::thread([this] {
        boost::system::error_code ec;
        io_.run(ec);
    });
}

void NetworkTask::stop()
{
    do_stop();
    if (!io_.stopped())
    {
        io_.stop();
    }
    if (thread_.joinable())
    {
        thread_.join();
    }
}
