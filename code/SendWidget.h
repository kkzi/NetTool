#pragma once

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
    void Draw()
    {
        ImGui::RadioButton("Text", &mode_, 0);
        ImGui::SameLine();
        ImGui::RadioButton("Hex", &mode_, 1);
        ImGui::SameLine(0, 40);
        if (ImGui::Button("CLEAR"))
        {
            *buffer_ = { 0 };
        }
        ImGui::SameLine(0, 20);
        if (ImGui::Button("SEND"))
        {
            auto bytes = ReadBytes();
            send_(std::move(bytes));
        }

        ImGui::InputTextMultiline("##send_editor", buffer_, IM_ARRAYSIZE(buffer_), ImGui::GetContentRegionAvail());
    }

private:
    std::vector<std::byte> ReadBytes()
    {
        std::vector<std::byte> bytes;
        switch (mode_)
        {
        case 0: {
            std::transform(buffer_, buffer_ + strlen(buffer_), std::back_inserter(bytes), [](auto &&it) {
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
};
