#pragma once
#include "imgui.h"
#include <GLFW/glfw3.h>

class UIConfig {
public:
    static void initialize() {
        // Get primary monitor
        GLFWmonitor* monitor = glfwGetPrimaryMonitor();
        if (!monitor) return;
        
        const GLFWvidmode* mode = glfwGetVideoMode(monitor);
        if (!mode) return;

        // Calculate scale factor based on resolution
        float baseWidth = 1920.0f;
        float currentWidth = static_cast<float>(mode->width);
        scaleFactor = currentWidth / baseWidth;

        // Clamp to reasonable range
        if (scaleFactor < 1.0f) scaleFactor = 1.0f;
        if (scaleFactor > 2.0f) scaleFactor = 2.0f;
    }

    static float getScaleFactor() {
        return scaleFactor;
    }

    static ImVec2 scale(ImVec2 size) {
        return ImVec2(size.x * scaleFactor, size.y * scaleFactor);
    }

private:
    static float scaleFactor;
};
