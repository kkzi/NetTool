#pragma once

#include "NetworkConfig.h"
#include "NetworkTask.h"
#include <functional>
#include <string>
#include <vector>

class ProtocolConfigWidget
{
public:
    ProtocolConfigWidget(std::function<void(bool)> start_callback);
    ~ProtocolConfigWidget();

public:
    void Setup(const NetworkConfig &);
    void Draw();
    void UpdateTaskStatus(NetworkTask::WorkState state);
    NetworkConfig Config() const;

private:
    std::vector<const char *> protocols_;
    std::vector<const char *> local_ips_;
    bool start_busy_{ false };
    bool start_toggled_{ false };
    std::function<void(bool)> callback_{ nullptr };

    int protocol_{ 0 };
    int local_ip_{ 0 };
    char local_port_[6]{ "0" };
    char remote_ip_[16]{ "127.0.0.1\0" };
    char remote_port_[6]{ "0" };
};
