#pragma once
#include "AWindow.h"

enum class BootPhase {
    INIT,
    CPU_CHECK,
    RAM_CHECK,
    DISK_CHECK,
    LOADING_OS,
    DONE
};

class BIOSBootWindow : public AWindow {
public:
    BIOSBootWindow();
    void draw() override;

private:
    BootPhase currentPhase;
    float phaseProgress;
    void advanceBootSequence();
};
