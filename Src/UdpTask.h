#pragma once

#include "NetworkTask.h"

class UdpTask : public NetworkTask
{
public:
    void start() override;
    void send(const QByteArray &) override;
};
