#pragma once

#include "NetworkTask.h"

class TcpClientTask : public NetworkTask
{

public:
    void start() override;
    void send(const QByteArray&) override;
};


