#pragma once

#include <string>

enum class Protocol : int
{
    TcpServer,
    TcpClient,
    UdpReceiver,
    UdpSender,
    Multicast,
};

struct NetworkConfig
{
    Protocol protocol;
    std::string local_ip;
    int local_port{ 0 };
    std::string remote_ip;
    int remote_port{ 0 };
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
