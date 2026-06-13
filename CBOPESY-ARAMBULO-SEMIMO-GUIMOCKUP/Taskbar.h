#pragma once
#include "AWindow.h"

class Taskbar : public AWindow {
public:
    Taskbar();
    void draw() override;

private:
    void drawSystemTray();
    void drawAppIcons();

    bool isStartMenuOpen = false;
    bool showFolderDummy = false;
    bool showInitDummy = false;
    bool showStopDummy = false;

    unsigned int folderIcon;
    unsigned int initIcon;
    unsigned int startIcon;
    unsigned int stopIcon;
    unsigned int chartIcon;
    unsigned int searchIcon;
};
