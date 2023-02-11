#include "NetworkTask.h"

NetworkTask::~NetworkTask()
{
}

void NetworkTask::start()
{
    doStart(io_);
    thread_ = std::thread([this] {
        boost::system::error_code ec;
        io_.run(ec);
    });
}

void NetworkTask::stop()
{
    doStop();
    // io_.reset();
     io_.stop();
    if (thread_.joinable())
    {
        thread_.join();
    }
}
