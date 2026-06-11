#pragma once
#include "AWindow.h"

class Taskbar : public AWindow {
public:
    Taskbar();
    void draw() override;

private:
    void drawSystemTray();
    void drawAppIcons();
};
