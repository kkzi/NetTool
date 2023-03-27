#include "App.h"
#include <hello_imgui/hello_imgui.h>
#include <iostream>
//#include <simple/use_spdlog.hpp>

int main(int argc, char **argv)
{
    std::string_view name{ "net assistant" };
    // init_logger(name);

    App app(name, argc, argv);
    return app.run();
}
