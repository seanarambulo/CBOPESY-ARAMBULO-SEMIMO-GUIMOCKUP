#pragma once
#include "AWindow.h"
#include <string>

class ProcessWindow : public AWindow {
public:
    ProcessWindow();
    void draw() override;

private:
    std::string processName;
    int pid;
    float cpuUsage;
    float memoryUsage;
    bool isPaused;
    int currentPriority;
};
