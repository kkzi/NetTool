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

constexpr static const char *CONFIG_FILE_NAME{ "./config.ini" };

constexpr static const char *KEY_THEME{ "Theme" };
constexpr static const char *KEY_LANG{ "Language" };

constexpr static const char *KEY_PROTOCOL{ "Protocol" };
constexpr static const char *KEY_LOCAL_IP{ "LocalIP" };
constexpr static const char *KEY_LOCAL_PORT{ "LocalPort" };
constexpr static const char *KEY_REMOTE_IP{ "RemoteIP" };
constexpr static const char *KEY_REMOTE_PORT{ "RemotePort" };
constexpr static const char *KEY_MULTICAST_IP{ "MulticastIP" };
