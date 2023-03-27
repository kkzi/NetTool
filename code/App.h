#pragma once

#include "ProtocolConfigWidget.h"
#include <string_view>

class App
{
public:
    App(std::string_view title, int argc, char **argv);

public:
    int run();

private:
    void showMainWindow();

private:
    std::string title_;

    ProtocolConfigWidget protocol_cfg_;
};
