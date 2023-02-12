#pragma once

#include <QStringList>
#include <QtNetwork/QHostAddress>
#include <QtNetwork/QNetworkInterface>

static QStringList getLocalIpAddress()
{
    QStringList arr{ "0.0.0.0" };
    for (auto it : QNetworkInterface::allInterfaces())
    {
        auto flags = it.flags();
        if (flags.testFlag(QNetworkInterface::IsRunning) && flags.testFlag(QNetworkInterface::IsUp))
        {
            for (auto ait : it.addressEntries())
            {
                if (ait.ip().protocol() == QAbstractSocket::IPv4Protocol)
                {
                    arr.push_back(ait.ip().toString());
                }
            }
        }
    }
    arr.sort();
    return arr;
}
