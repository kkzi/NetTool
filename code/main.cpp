#include "App.h"
#include <imgui.h>
#include <iostream>

// int main(int argc, char **argv)
int WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR lpCmdLine, int nShowCmd)
{
    App app("NET ASSISTANT v2.0 beta1");
    return app.run();
}
