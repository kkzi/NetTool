#pragma once

#include <boost/locale.hpp>
#include <imgui.h>
#include <ranges>
#include <sstream>
#include <vector>

class RecvWidget
{

public:
    RecvWidget()
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
            buffer_.clear();
        }
        ImGui::SameLine(0, 20);
        if (ImGui::SmallButton("COPY"))
        {
            ImGui::LogToClipboard();
            auto text = get_text_string();
            ImGui::LogText((char *)text.c_str());
            ImGui::LogFinish();
        }

        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 1);
        ImGui::PushStyleColor(ImGuiCol_ChildBg, { .98f, .98f, .98f, 1 });
        ImGui::BeginChild("receive_edit", { -16, -8 }, true, 0);

        draw_text();

        ImGui::EndChild();
        ImGui::PopStyleColor();
        ImGui::PopStyleVar();
    }

    void AppendBuffer(const std::vector<std::byte> &data)
    {
        static constexpr size_t BUFFER_LIMIT = 1024;
        std::copy((char *)data.data(), (char *)data.data() + data.size(), std::back_inserter(buffer_));
        if (buffer_.size() > BUFFER_LIMIT)
        {
            buffer_.erase(0, buffer_.size() - BUFFER_LIMIT);
        }
    }

private:
    std::string get_text_string()
    {
        if (buffer_.empty())
        {
            return {};
        }
        switch (mode_)
        {
        case 0: {
            return buffer_;
        }
        case 1: {
            std::stringstream ss;
            std::ranges::for_each(buffer_, [&ss](auto &&c) {
                ss << std::format("{:02X} ", uint8_t(c));
            });
            return ss.str();
        }
        default:
            return {};
        }
    }

    void draw_text()
    {
        auto text = get_text_string();
        if (text.empty())
        {
            return;
        }
        auto is_hex = mode_ == 1;
        if (is_hex)
        {
            ImGui::PushFont(mono_);
        }
        ImGui::TextWrapped(text.c_str());
        if (is_hex)
        {
            ImGui::PopFont();
        }
    }

    static std::string gbk2utf8(const std::string &input)
    {
        int len = MultiByteToWideChar(CP_ACP, 0, input.c_str(), -1, NULL, 0);
        std::wstring wstr(len, '0');
        MultiByteToWideChar(CP_ACP, 0, input.c_str(), -1, wstr.data(), len);

        len = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, NULL, 0, NULL, NULL);
        std::string output(len, '0');
        WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, output.data(), len, NULL, NULL);
        return output;
    }

private:
    int mode_{ 0 };
    std::string buffer_;
    ImFont *mono_{ nullptr };
};
