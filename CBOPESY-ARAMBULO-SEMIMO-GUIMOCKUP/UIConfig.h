#pragma once
#include "imgui.h"

class UIConfig {
private:
    static UIConfig* instance;
    float scaleFactor;
    float baseWidth;

    UIConfig() : scaleFactor(1.0f), baseWidth(1920.0f) {}

public:
    static UIConfig* getInstance() {
        if (instance == nullptr) {
            instance = new UIConfig();
        }
        return instance;
    }

    void initialize(float currentMonitorWidth) {
        scaleFactor = currentMonitorWidth / baseWidth;
    }

    float getScaleFactor() const {
        return scaleFactor;
    }
};
