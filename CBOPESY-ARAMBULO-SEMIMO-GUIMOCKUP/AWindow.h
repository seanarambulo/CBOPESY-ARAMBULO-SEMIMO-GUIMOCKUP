#pragma once
#include <string>
#include "imgui.h"

class AWindow {
public:
    AWindow(const std::string& name) : windowName(name), isVisible(false) {}
    virtual ~AWindow() = default;

    virtual void draw() = 0; // Pure virtual: subclasses must implement

    void show() { isVisible = true; }
    void hide() { isVisible = false; }
    bool isShown() const { return isVisible; }

protected:
    bool beginWindow(ImGuiWindowFlags flags = 0) {
        if (!isVisible) return false;
        ImGui::Begin(windowName.c_str(), &isVisible, flags);
        return true;
    }

    void endWindow() {
        ImGui::End();
    }

    std::string windowName;
    bool isVisible;
};
