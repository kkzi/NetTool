#pragma once

#include "NetworkTask.h"

class UdpTask : public NetworkTask
{
public:
    void start(io_context &io) override;
    void send(const QByteArray &) override;
};
