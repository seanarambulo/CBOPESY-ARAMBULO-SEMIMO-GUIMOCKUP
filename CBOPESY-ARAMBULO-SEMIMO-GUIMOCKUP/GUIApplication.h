#pragma once
#include <memory>
#include <GLFW/glfw3.h>

class GUIApplication {
public:
    GUIApplication();
    ~GUIApplication();

    bool initialize();
    void run();

private:
    void updateLogic();
    void renderFrame();
    void shutdown();

    GLFWwindow* window;
};
