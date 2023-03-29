#pragma once

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
        logs_.emplace_back(msg);
        while (logs_.size() > 256)
        {
            logs_.erase(logs_.begin());
        }
    }

    void Draw()
    {
        //ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(8, 0));
        for (auto &&it : logs_)
        {
            ImGui::TextUnformatted(it.c_str());
        }
        //ImGui::PopStyleVar();
        if (auto_scroll_ && ImGui::GetScrollY() >= ImGui::GetScrollMaxY()) ImGui::SetScrollHereY(1.0f);
    }

private:
    std::vector<std::string> logs_;
    bool auto_scroll_{ true };
};
