#include "ProtocolConfigWidget.h"
#include "NetworkConfig.h"
#include <boost/asio.hpp>
#include <hello_imgui/hello_imgui.h>
#include <ranges>

static std::vector<const char *> get_local_ips()
{
    std::vector<const char *> ips;
    try
    {
        namespace ip = boost::asio::ip;
        boost::asio::io_context io;
        ip::tcp::resolver resolver(io);
        for (auto &&it : resolver.resolve(ip::host_name(), ""))
        {
            if (it.endpoint().address().is_v4())
            {
                std::string addr = it.endpoint().address().to_string();
                auto text = new char[1 + addr.size()]{ 0 };
                std::ranges::copy(addr, text);
                ips.push_back(text);
            }
        }
    }
    catch (...)
    {
    }
    return ips;
}

ProtocolConfigWidget::ProtocolConfigWidget(std::function<void(bool)> callback)
    : callback_(std::move(callback))
{
    protocols_ = { "TCP Server", "TCP Client", "UDP", "UDP Multicast" };
    local_ips_ = get_local_ips();
    local_ips_.insert(local_ips_.begin(), new char[]{ "127.0.0.1\0" });
}

ProtocolConfigWidget::~ProtocolConfigWidget()
{
    for (auto &it : local_ips_)
    {
        delete[] it;
    }
}

void ProtocolConfigWidget::Draw()
{
    constexpr static int WIDTH_PROTO = 120;
    constexpr static int WIDTH_IP = 140;
    constexpr static int WIDTH_PORT = 64;

    ImGui::AlignTextToFramePadding();
    ImGui::Text("Protocol");

    ImGui::SameLine();
    ImGui::PushItemWidth(WIDTH_PROTO);
    ImGui::Combo("##protocol", &protocol_, protocols_.data(), (int)protocols_.size());
    ImGui::PopItemWidth();

    ImGui::SameLine();
    ImGui::Text("Local IP");

    ImGui::SameLine();
    ImGui::PushItemWidth(WIDTH_IP);
    ImGui::Combo("##local_ip", &local_ip_, local_ips_.data(), (int)local_ips_.size());
    ImGui::PopItemWidth();

    if (protocol_ == (int)Protocol::Multicast)
    {
        ImGui::SameLine();
        ImGui::Text("Multicast IP");
        ImGui::SameLine();
        ImGui::PushItemWidth(WIDTH_IP);
        ImGui::InputText("##multicast_ip", remote_ip_, IM_ARRAYSIZE(remote_ip_));
        ImGui::PopItemWidth();
    }

    ImGui::SameLine();
    ImGui::Text("Port");

    ImGui::PushItemWidth(WIDTH_PORT);
    ImGui::SameLine();
    ImGui::InputText("##local_port", local_port_, IM_ARRAYSIZE(local_port_));
    ImGui::PopItemWidth();

    if (protocol_ == (int)Protocol::TcpClient || protocol_ == (int)Protocol::Udp)
    {
        ImGui::SameLine();
        ImGui::Text("Remote IP");

        ImGui::SameLine();
        ImGui::PushItemWidth(WIDTH_IP);
        ImGui::InputText("##remote_ip", remote_ip_, IM_ARRAYSIZE(remote_ip_));
        ImGui::PopItemWidth();

        ImGui::SameLine();
        ImGui::Text("Port");

        ImGui::PushItemWidth(WIDTH_PORT);
        ImGui::SameLine();
        ImGui::InputText("##remote_port", remote_port_, IM_ARRAYSIZE(remote_port_));
        ImGui::PopItemWidth();
    }
    ImGui::SameLine(0, 20);

    ImGui::BeginDisabled(start_busy_);
    if (ImGui::Button(!start_busy_ && start_toggled_ ? "STOP" : "START", { 64, 0 }))
    {
        start_busy_ = true;
        start_toggled_ = !start_toggled_;
        callback_(start_toggled_);
    }
    ImGui::EndDisabled();
}

void ProtocolConfigWidget::update_task_state(NetworkTask::WorkState state)
{
    start_busy_ = false;
    start_toggled_ = state == NetworkTask::WorkState::OK;
}

NetworkConfig ProtocolConfigWidget::config() const
{
    return NetworkConfig{
        .protocol = Protocol(protocol_),
        .local_ip = local_ips_.at(local_ip_),
        .local_port = std::atoi(local_port_),
        .remote_ip = remote_ip_,
        .remote_port = std::atoi(remote_port_),
    };
}
