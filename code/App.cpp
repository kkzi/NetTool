#include "App.h"
#include <hello_imgui/hello_imgui.h>
#include <string>

App::App(std::string_view title, int argc, char **argv)
    : title_(title)
{
}

int App::run()
{
    HelloImGui::RunnerParams params;
    params.callbacks.ShowGui = [this] {
        showMainWindow();
    };
    params.callbacks.LoadAdditionalFonts = [] {
        auto &io = ImGui::GetIO();
        io.Fonts->AddFontFromFileTTF("C:/Windows/Fonts/msyh.ttc", 18, nullptr, io.Fonts->GetGlyphRangesChineseSimplifiedCommon());
    };
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

void App::showMainWindow()
{
    protocol_cfg_.update();
}
