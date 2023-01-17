#pragma once

#include "NetworkTask.h"

class MulticastTask : public NetworkTask
{
public:
    void start() override;
    void send(const QByteArray &) override;
};
