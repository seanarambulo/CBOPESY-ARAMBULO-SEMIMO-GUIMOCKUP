// CBOPESY-ARAMBULO-SEMIMO-GUIMOCKUP.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "GUIApplication.h"

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
