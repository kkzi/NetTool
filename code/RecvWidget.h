#pragma once

#include "TextEditor.h"
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

        auto flags = ImGuiInputTextFlags_ReadOnly | ImGuiInputTextFlags_AutoSelectAll;
        switch (mode_)
        {
        case 0: {
            editor_.SetText(buffer_);
            ImGui::InputTextMultiline("##recv_editor", buffer_.data(), buffer_.size() + 1, ImGui::GetContentRegionAvail(), flags);
            break;
        }
        case 1: {
            std::stringstream ss;
            std::ranges::for_each(buffer_, [&ss](auto &&c) {
                ss << std::format("{:02X} ", uint16_t(c));
            });

            auto text = ss.str();
            // auto draw_list = ImGui::GetWindowDrawList();
            // draw_list->AddText(ImGui::GetFont(), ImGui::GetFontSize(), ImVec2{0, 0}, ImU32(0), text.data(), text.data() + text.size(), 640);
            ImGui::InputTextMultiline("##recv_editor", text.data(), text.size(), ImGui::GetContentRegionAvail(), flags);
            // ImGui::TextWrapped(text.c_str());
            break;
        }
        default:
            break;
        }
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
    TextEditor editor_;
};
