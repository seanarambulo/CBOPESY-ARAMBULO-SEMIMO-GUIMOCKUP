#pragma once
#include "AWindow.h"

class Desktop : public AWindow {
public:
    Desktop();
    void draw() override;

private:
    void drawClock();
    unsigned int wallpaperTexture;
    int texWidth, texHeight;
};
