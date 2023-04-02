#pragma once

#include <chrono>
#include <format>
#include <imgui.h>
#include <string_view>
#include <vector>

class LogWidget
{

public:
    void Clear()
    {
        logs_.clear();
    }

    void AddLog(std::string_view msg)
    {
        logs_.emplace_back(std::format("{}  {}", std::chrono::system_clock::now(), msg));
        while (logs_.size() > 256)
        {
            logs_.erase(logs_.begin());
        }
    }

    void Draw()
    {
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 1);
        ImGui::PushStyleColor(ImGuiCol_ChildBg, { 0.95f, 0.95f, 0.95f, 1.00f });
        ImGui::BeginChild("log edit", { -16, -8 }, true, 0);
        // ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(8, 0));
        for (auto &&it : logs_)
        {
            ImGui::TextUnformatted(it.c_str());
        }
        // ImGui::PopStyleVar();
        if (auto_scroll_ && ImGui::GetScrollY() >= ImGui::GetScrollMaxY()) ImGui::SetScrollHereY(1.0f);

        ImGui::EndChild();
        ImGui::PopStyleColor();
        ImGui::PopStyleVar();
    }

private:
    std::vector<std::string> logs_;
    bool auto_scroll_{ true };
};
