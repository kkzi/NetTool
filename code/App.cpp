#include "App.h"
#include "SimpleLayout.h"
#include "TcpServerTask.h"
#include <hello_imgui/hello_imgui.h>
#include <hello_imgui/widgets/logger.h>
#include <string>

App::App(std::string_view title, int argc, char **argv)
    : title_(title)
    , cfg_widget_(std::bind(&App::ctrl_task, this, std::placeholders::_1))
{
}

int App::run()
{
    HelloImGui::RunnerParams params;
    params.callbacks.ShowGui = [this] {
        show_main_window();
    };
    params.callbacks.SetupImGuiConfig = [] {
        ImGui::GetIO().IniFilename = "";
    };
    // params.callbacks.LoadAdditionalFonts = [] {
    //    auto &io = ImGui::GetIO();
    //    io.IniFilename = "";
    //    io.Fonts->AddFontFromFileTTF("C:/Windows/Fonts/msyh.ttc", 16, nullptr, io.Fonts->GetGlyphRangesChineseSimplifiedCommon());
    //};
    params.appWindowParams = {
        .windowTitle = title_,
        .windowGeometry = {
            .size = {960, 640},
            .sizeAuto = false,
            .positionMode = HelloImGui::WindowPositionMode::MonitorCenter,
        },
    };
    // params.imGuiWindowParams.tweakedTheme.Theme = ImGuiTheme::ImGuiTheme_WhiteIsWhite;
    params.imGuiWindowParams.tweakedTheme.Theme = ImGuiTheme::ImGuiTheme_MicrosoftStyle;

    HelloImGui::Run(params);

    return EXIT_SUCCESS;
}

void App::show_main_window()
{
    ImGui::Dummy({ 0, 8 });
    ImGui::Indent(12);

    // ImGui::PushStyleColor(ImGuiCol_Border, 0xFFFFFF);
    // ImGui::PushStyleVar(ImGuiStyleVar_ChildBorderSize, 0);

    auto width = ImGui::GetWindowSize().x - 24;
    auto height = ImGui::GetWindowHeight();

    auto flags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse;
    bool show_border = false;

    cfg_widget_.update();

    // static SimpleLayout sl;
    // sl.Draw();

    ImGui::BeginChild("receive_widget", { width, height * 0.4f }, show_border, flags);
    recv_widget_.update();
    auto n = ImGui::GetContentRegionMax().y;
    ImGui::EndChild();

    ImGui::BeginChild("send_widget", { width, height * 0.2f }, show_border, flags);
    send_widget_.update();
    ImGui::EndChild();

    ImGui::BeginChild("log_widget", { width, -ImGui::GetFrameHeight() + 10 }, show_border, flags);
    log_widget_.update();
    ImGui::EndChild();

    // ImGui::PopStyleVar();
    // ImGui::PopStyleColor();
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

std::shared_ptr<NetworkTask> App::create_task() const
{
    NetworkConfig cfg = cfg_widget_.config();

    switch (cfg.protocol)
    {
    case Protocol::TcpServer:
        return std::make_shared<TcpServerTask>(cfg);
    case Protocol::TcpClient:
    case Protocol::Udp:
    case Protocol::Multicast:
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
}

void App::on_message_received(std::string_view msg)
{
    log_widget_.AddLog(msg.data());
}
