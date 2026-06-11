#include "BIOSBootWindow.h"

BIOSBootWindow::BIOSBootWindow() : AWindow("BIOSBootWindow"), currentPhase(BootPhase::INIT), phaseProgress(0.0f) {
}

void BIOSBootWindow::advanceBootSequence() {
    if (currentPhase == BootPhase::DONE) {
        return;
    }
    
    phaseProgress += ImGui::GetIO().DeltaTime * 0.5f; // Adjust speed here
    
    if (phaseProgress >= 1.0f) {
        phaseProgress = 0.0f;
        
        switch (currentPhase) {
            case BootPhase::INIT: currentPhase = BootPhase::CPU_CHECK; break;
            case BootPhase::CPU_CHECK: currentPhase = BootPhase::RAM_CHECK; break;
            case BootPhase::RAM_CHECK: currentPhase = BootPhase::DISK_CHECK; break;
            case BootPhase::DISK_CHECK: currentPhase = BootPhase::LOADING_OS; break;
            case BootPhase::LOADING_OS: currentPhase = BootPhase::DONE; break;
            default: break;
        }
    }
}

void BIOSBootWindow::draw() {
    if (!isVisible) {
        currentPhase = BootPhase::INIT; // Reset when hidden
        phaseProgress = 0.0f;
        return;
    }
    
    beginWindow();
    
    advanceBootSequence();
    
    const char* phaseName = "";
    switch (currentPhase) {
        case BootPhase::INIT: phaseName = "Initializing..."; break;
        case BootPhase::CPU_CHECK: phaseName = "Checking CPU..."; break;
        case BootPhase::RAM_CHECK: phaseName = "Checking RAM..."; break;
        case BootPhase::DISK_CHECK: phaseName = "Checking Disks..."; break;
        case BootPhase::LOADING_OS: phaseName = "Loading OS..."; break;
        case BootPhase::DONE: phaseName = "Boot Complete!"; break;
    }
    
    ImGui::Text("%s", phaseName);
    
    if (currentPhase != BootPhase::DONE) {
        ImGui::ProgressBar(phaseProgress, ImVec2(-1, 0), phaseName);
    } else {
        ImGui::TextColored(ImVec4(0.2f, 1.0f, 0.2f, 1.0f), "System is ready.");
        if (ImGui::Button("Close")) {
            isVisible = false;
        }
    }
    
    endWindow();
}
