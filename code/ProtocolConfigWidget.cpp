#include "ProtocolConfigWidget.h"
#include <hello_imgui/hello_imgui.h>

void ProtocolConfigWidget::update()
{
    ImGui::AlignTextToFramePadding();
    ImGui::PushItemWidth(140);

    ImGui::Text("Protocol");

    ImGui::SameLine();
    const char *protocol_items[] = { "TCP Server", "TCP Client", "UDP", "UDP Multicast" };
    ImGui::Combo("##protocol", &protocol, protocol_items, IM_ARRAYSIZE(protocol_items));

    ImGui::SameLine();
    ImGui::Text("Local IP");

    ImGui::SameLine();
    const char *local_ip_items[] = { "0.0.0.0", "127.0.0.1" };
    ImGui::Combo("##local_ip", &local_ip, local_ip_items, IM_ARRAYSIZE(local_ip_items));

    ImGui::SameLine();
    ImGui::Text("Port");

    ImGui::PopItemWidth();
    ImGui::PushItemWidth(120);
    ImGui::SameLine();
    ImGui::InputInt("##local_port", &local_port);

    ImGui::PopItemWidth();
    ImGui::PushItemWidth(140);
    ImGui::SameLine();
    ImGui::Text("Remote IP");

    ImGui::SameLine();
    ImGui::InputText("##remote_ip", remote_ip, strlen(remote_ip));

    ImGui::SameLine();
    ImGui::Text("Port");

    ImGui::PopItemWidth();
    ImGui::PushItemWidth(120);
    ImGui::SameLine();
    ImGui::InputInt("##remote_port", &remote_port);

    ImGui::SameLine();
    if (ImGui::ColorButton("START", {}))
    {
    }
}
