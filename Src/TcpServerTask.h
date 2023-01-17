#pragma once

#include "NetworkTask.h"


class TcpServerTask : public NetworkTask
{

public:
    void start() override;
    void send(const QByteArray&) override;
};

