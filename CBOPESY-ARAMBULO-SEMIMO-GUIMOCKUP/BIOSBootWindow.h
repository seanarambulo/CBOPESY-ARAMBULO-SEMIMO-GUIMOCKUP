#pragma once
#include "AWindow.h"
#include <chrono>

class BIOSBootWindow : public AWindow {
public:
    BIOSBootWindow();
    void draw() override;

private:
    std::chrono::time_point<std::chrono::system_clock> startTime;
    int ramCounter;
};
