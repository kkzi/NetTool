#pragma once

#include <imgui.h>

class RecvWidget
{

public:
    void update()
    {
        ImGui::BeginGroup();
        ImGui::AlignTextToFramePadding();
        ImGui::Text("Received");
        ImGui::SameLine();
        ImGui::RadioButton("Text", &mode_, 0);
        ImGui::SameLine();
        ImGui::RadioButton("Hex", &mode_, 1);
        ImGui::SameLine();
        if (ImGui::Button("CLEAR"))
        {
        }

        auto flags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse;
        ImGui::BeginChild("Receive Buffer", { 0, 100 }, false, flags);
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
        // const char *buf = Buf.begin();
        // const char *buf_end = Buf.end();
        // ImGuiListClipper clipper;
        // clipper.Begin(LineOffsets.Size);
        // while (clipper.Step())
        //{
        //    for (int line_no = clipper.DisplayStart; line_no < clipper.DisplayEnd; line_no++)
        //    {
        //        const char *line_start = buf + LineOffsets[line_no];
        //        const char *line_end = (line_no + 1 < LineOffsets.Size) ? (buf + LineOffsets[line_no + 1] - 1) : buf_end;
        //        ImGui::TextUnformatted(line_start, line_end);
        //    }
        //}
        // clipper.End();

        ImGui::PopStyleVar();
        ImGui::EndChild();
        ImGui::EndGroup();
    }

private:
    int mode_{ 0 };
};
