#pragma once

#include <string>

struct ProtocolConfigWidget
{
    int protocol{ 0 };
    int local_ip{ 0 };
    int local_port{ 0 };
    char remote_ip[64]{ 0 };
    int remote_port{ 0 };

    void update();
};
