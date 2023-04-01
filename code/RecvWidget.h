#pragma once

#include <imgui.h>
#include <ranges>
#include <sstream>

class RecvWidget
{

public:
    void Draw()
    {
        ImGui::RadioButton("Text", &mode_, 0);
        ImGui::SameLine();
        ImGui::RadioButton("Hex", &mode_, 1);
        ImGui::SameLine(0, 40);
        if (ImGui::Button("CLEAR"))
        {
            buffer_.clear();
        }

        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 1);
        ImGui::PushStyleColor(ImGuiCol_ChildBg, { .98f, .98f, .98f, 1 });
        ImGui::BeginChild("receive_edit", { -16, -8 }, true, 0);

        switch (mode_)
        {
        case 0: {
            ImGui::TextWrapped(buffer_.c_str());
            break;
        }
        case 1: {
            std::stringstream ss;
            std::ranges::for_each(buffer_, [&ss](auto &&c) {
                ss << std::format("{:02X} ", uint16_t(c));
            });
            auto text = ss.str();
            ImGui::TextWrapped(text.c_str());
            break;
        }
        default:
            break;
        }
        ImGui::EndChild();
        ImGui::PopStyleColor();
        ImGui::PopStyleVar();
    }

    void AppendBuffer(const std::string &data)
    {
        static constexpr size_t BUFFER_LIMIT = 1024;
        std::ranges::copy(data, std::back_inserter(buffer_));
        if (buffer_.size() > BUFFER_LIMIT)
        {
            buffer_.erase(0, buffer_.size() - BUFFER_LIMIT);
        }
    }

private:
    int mode_{ 0 };
    std::string buffer_;
};
