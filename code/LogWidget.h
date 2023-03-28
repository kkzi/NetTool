#pragma once

#include <imgui.h>

class LogWidget
{

public:
    void Clear()
    {
        Buf.clear();
        LineOffsets.clear();
        LineOffsets.push_back(0);
    }

    void AddLog(const char *msg)
    {
        int old_size = Buf.size();
        Buf.append(msg);
        Buf.append("\n");
        for (int new_size = Buf.size(); old_size < new_size; old_size++)
        {
            if (Buf[old_size] == '\n') LineOffsets.push_back(old_size + 1);
        }
    }

    void update()
    {
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
        const char *buf = Buf.begin();
        const char *buf_end = Buf.end();
        ImGuiListClipper clipper;
        clipper.Begin(LineOffsets.Size);
        while (clipper.Step())
        {
            for (int line_no = clipper.DisplayStart; line_no < clipper.DisplayEnd; line_no++)
            {
                const char *line_start = buf + LineOffsets[line_no];
                const char *line_end = (line_no + 1 < LineOffsets.Size) ? (buf + LineOffsets[line_no + 1] - 1) : buf_end;
                ImGui::TextUnformatted(line_start, line_end);
            }
        }
        clipper.End();
        ImGui::PopStyleVar();

        if (AutoScroll && ImGui::GetScrollY() >= ImGui::GetScrollMaxY()) ImGui::SetScrollHereY(1.0f);
    }

private:
    ImGuiTextBuffer Buf;
    ImVector<int> LineOffsets;
    bool AutoScroll{ true };
};
