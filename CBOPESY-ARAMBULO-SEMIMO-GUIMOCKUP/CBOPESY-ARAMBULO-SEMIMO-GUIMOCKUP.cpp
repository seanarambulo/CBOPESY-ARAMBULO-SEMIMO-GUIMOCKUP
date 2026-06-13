// CBOPESY-ARAMBULO-SEMIMO-GUIMOCKUP.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <chrono>
#include "GUIApplication.h"

#ifdef _WIN32
#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
#endif

int main()
{
    GUIApplication app;
    if (app.initialize()) {
        app.run();
    } else {
        std::cerr << "Failed to initialize GUI Application!" << std::endl;
        return -1;
    }
    return 0;
}
