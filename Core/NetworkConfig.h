#pragma once

#include <QString>


struct NetworkConfig
{
    QString protocol;
    QString localIp;
    int localPort{ 0 };
    QString remoteIp;
    int remotePort{ 0 };
    QString multicastIp;
};


