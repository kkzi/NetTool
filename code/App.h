#pragma once

#include "LogWidget.h"
#include "NetworkTask.h"
#include "ProtocolConfigWidget.h"
#include "RecvWidget.h"
#include "SendWidget.h"
#include <memory>
#include <string_view>

class App
{
public:
    App(std::string_view title, int argc, char **argv);

public:
    int run();

private:
    void show_main_window();
    void ctrl_task(bool on);
    void send_bytes(const std::vector<std::byte> &bytes);
    std::shared_ptr<NetworkTask> create_task() const;

    void on_state_changed(NetworkTask::WorkState state);
    void on_data_received(std::string_view from, const std::vector<std::byte> &data);
    void on_message_received(std::string_view msg);

private:
    std::string title_;
    ProtocolConfigWidget cfg_widget_;
    RecvWidget recv_widget_;
    SendWidget send_widget_;
    LogWidget log_widget_;
    std::shared_ptr<NetworkTask> task_{ nullptr };
};
