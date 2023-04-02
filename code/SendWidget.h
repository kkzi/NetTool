#pragma once

#include <boost/locale.hpp>
#include <imgui.h>
#include <ranges>
#include <sstream>
#include <vector>

class SendWidget
{
public:
    SendWidget(std::function<void(const std::vector<std::byte> &)> func)
        : send_(std::move(func))
    {
    }

public:
    void Setup(ImFont *mono)
    {
        mono_ = mono;
    }

    void Draw()
    {
        ImGui::RadioButton("Text", &mode_, 0);
        ImGui::SameLine();
        ImGui::RadioButton("Hex", &mode_, 1);
        ImGui::SameLine(0, 40);
        if (ImGui::SmallButton("CLEAR"))
        {
            *buffer_ = { 0 };
        }
        ImGui::SameLine(0, 20);
        if (ImGui::SmallButton("SEND"))
        {
            auto bytes = read_bytes();
            send_(std::move(bytes));
        }

        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 1);
        ImGui::PushStyleColor(ImGuiCol_ChildBg, { .98f, .98f, .98f, 1 });
        ImGui::BeginChild("receive_edit", { -16, -8 }, true, 0);

        auto hex_mode = mode_ == 1;
        if (hex_mode)
        {
            ImGui::PushFont(mono_);
        }
        ImGui::InputTextMultiline("##send_editor", buffer_, IM_ARRAYSIZE(buffer_), ImGui::GetContentRegionAvail());
        if (hex_mode)
        {
            ImGui::PopFont();
        }

        ImGui::EndChild();
        ImGui::PopStyleColor();
        ImGui::PopStyleVar();
    }

private:
    std::vector<std::byte> read_bytes()
    {
        std::vector<std::byte> bytes;
        switch (mode_)
        {
        case 0: {
            auto gbk = boost::locale::conv::to_utf<char>(buffer_, "utf-8");
            std::ranges::transform(gbk, std::back_inserter(bytes), [](auto &&it) {
                return std::byte(it);
            });
            break;
        }
        case 1: {
            for (auto i = 0; i < strlen(buffer_);)
            {
                if (std::isblank(buffer_[i]))
                {
                    i += 1;
                    continue;
                }
                unsigned char byte = 0;
                auto n = std::from_chars(buffer_ + i, buffer_ + i + 2, byte, 16);
                bytes.push_back(std::byte(byte));
                i += 2;
            }
            break;
        }
        default:
            break;
        }
        return bytes;
    }

private:
    std::function<void(const std::vector<std::byte> &)> send_;
    int mode_{ 0 };
    char buffer_[4096]{ 0 };
    ImFont *mono_{ nullptr };
};
