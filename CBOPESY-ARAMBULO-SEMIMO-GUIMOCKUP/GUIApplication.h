#pragma once
#include <memory>
#include <GLFW/glfw3.h>
#include "Desktop.h"
#include "Taskbar.h"

class GUIApplication {
public:
    GUIApplication();
    ~GUIApplication();

    bool initialize();
    void run();

private:
    void setupImGui();
    void updateLogic();
    void renderFrame();
    void shutdown();

    GLFWwindow* window;
    std::shared_ptr<Desktop> desktop;
    std::shared_ptr<Taskbar> taskbar;
};
