#include "App.h"
#include "MulticastTask.h"
#include "TcpClientTask.h"
#include "TcpServerTask.h"
#include "UdpTask.h"
#include "use_imgui_dx12.hpp"
#include <ranges>
#include <string>

App::App(std::string_view title)
    : title_(title)
    , cfg_widget_(std::bind(&App::ctrl_task, this, std::placeholders::_1))
    , send_widget_(std::bind(&App::send_bytes, this, std::placeholders::_1))
{
}

int App::run()
{
    ImGuiDx12::RunOptions opts;
    opts.Title.clear();
    std::ranges::copy(title_, std::back_inserter(opts.Title));

    ImGuiDx12::Run(opts, [&] {
        show_main_window();
    });
    return EXIT_SUCCESS;
}

void App::show_main_window()
{
    cfg_widget_.Draw();

    auto size = ImGui::GetWindowSize();
    bool show_border = false;
    ImGui::SeparatorText("RECEIVE");
    if (ImGui::BeginChild("receive_widget", { size.x, size.y * 0.4f }, show_border))
    {
        recv_widget_.Draw();
        ImGui::EndChild();
    }

    ImGui::SeparatorText("SEND");
    if (ImGui::BeginChild("send_widget", { size.x, size.y * 0.24f }, show_border))
    {
        send_widget_.Draw();
        ImGui::EndChild();
    }

    ImGui::SeparatorText("LOG");
    if (ImGui::BeginChild("log_widget", { size.x, 0 }, show_border))
    {
        log_widget_.Draw();
        ImGui::EndChild();
    }
}

void App::ctrl_task(bool on)
{
    if (on)
    {
        task_ = create_task();
        assert(task_ != nullptr);
        task_->on_state_changed([this](auto &&state) {
            on_state_changed(std::move(state));
        });
        task_->on_message([this](auto &&msg) {
            on_message_received(std::move(msg));
        });
        task_->on_data([this](auto &&from, auto &&data) {
            on_data_received(std::move(from), std::move(data));
        });
        task_->start();
    }
    else if (task_ != nullptr)
    {
        task_->stop();
        cfg_widget_.update_task_state(NetworkTask::WorkState::FAILED);
        task_.reset();
    }
}

void App::send_bytes(const std::vector<std::byte> &bytes)
{
    if (bytes.empty())
    {
        return;
    }
    if (task_ == nullptr)
    {
        return;
    }
    task_->send(bytes);
}

std::shared_ptr<NetworkTask> App::create_task() const
{
    NetworkConfig cfg = cfg_widget_.config();

    switch (cfg.protocol)
    {
    case Protocol::TcpServer:
        return std::make_shared<TcpServerTask>(cfg);
    case Protocol::TcpClient:
        return std::make_shared<TcpClientTask>(cfg);
    case Protocol::Udp:
        return std::make_shared<UdpTask>(cfg);
    case Protocol::Multicast:
        return std::make_shared<MulticastTask>(cfg);
    default:
        break;
    }
    return nullptr;
}

void App::on_state_changed(NetworkTask::WorkState state)
{
    cfg_widget_.update_task_state(state);
}

void App::on_data_received(std::string_view from, const std::vector<std::byte> &data)
{
    recv_widget_.AppendBuffer(std::string((char *)data.data(), data.size()));
}

void App::on_message_received(std::string_view msg)
{
    log_widget_.AddLog(msg);
}
