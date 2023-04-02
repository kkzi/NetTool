#pragma once

#include "NetworkConfig.h"

#include <boost/property_tree/ini_parser.hpp>
#include <boost/property_tree/ptree.hpp>
#include <fstream>
#include <magic_enum.hpp>
#include <optional>

static std::optional<NetworkConfig> ReadConfig(std::string_view path = "./config.ini")
{
    namespace pt = boost::property_tree;

    std::ifstream input(path.data());
    if (!input)
    {
        return std::nullopt;
    }

    NetworkConfig cfg;
    pt::ptree tree;
    pt::read_ini(input, tree);
    auto general_node = tree.get_child("General");
    cfg.protocol = (Protocol)general_node.get<int>("Protocol");
    auto protocol_node = tree.get_child(magic_enum::enum_name(cfg.protocol).data());
    cfg.local_ip = protocol_node.get<std::string>("LocalIP", "");
    cfg.local_port = protocol_node.get<int>("LocalPort", 0);
    cfg.remote_ip = protocol_node.get<std::string>("RemoteIP", "");
    cfg.remote_port = protocol_node.get<int>("RemotePort", 0);
    return cfg;
}

static void SaveConfig(const NetworkConfig &cfg, std::string_view path = "./config.ini")
{
    namespace pt = boost::property_tree;
    std::ofstream output(path.data());
    pt::ptree tree;
    tree.put("General.Protocol", (int)cfg.protocol);

    pt::ptree protocol_node;
    protocol_node.put("LocalIP", cfg.local_ip);
    protocol_node.put("LocalPort", cfg.local_port);
    protocol_node.put("RemoteIP", cfg.remote_ip);
    protocol_node.put("RemotePort", cfg.remote_port);

    auto protocol_name = magic_enum::enum_name(cfg.protocol);
    tree.add_child(protocol_name.data(), protocol_node);
    pt::write_ini(output, tree);
}
