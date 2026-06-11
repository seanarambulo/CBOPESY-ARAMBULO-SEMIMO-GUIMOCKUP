#pragma once
#include <string>
#include "imgui.h"

class AWindow {
protected:
    std::string windowName;
    bool isVisible;

public:
    AWindow(const std::string& name) : windowName(name), isVisible(false) {}
    virtual ~AWindow() = default;

    std::string getName() const { return windowName; }
    bool getVisibility() const { return isVisible; }
    void setVisibility(bool visible) { isVisible = visible; }

    virtual void draw() = 0;

    void beginWindow() {
        if (isVisible) {
            ImGui::Begin(windowName.c_str(), &isVisible);
        }
    }

    void endWindow() {
        if (isVisible) {
            ImGui::End();
        }
    }
};
