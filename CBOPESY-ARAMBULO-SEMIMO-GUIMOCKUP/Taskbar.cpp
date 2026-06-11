#include "Taskbar.h"
#include "UIConfig.h"
#include "UIManager.h"
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>

Taskbar::Taskbar() : AWindow("Taskbar") {
    isVisible = true; // Taskbar is always visible
}

void Taskbar::drawAppIcons() {
    float scale = UIConfig::getInstance()->getScaleFactor();
    ImVec2 buttonSize(80 * scale, 30 * scale);
    
    if (ImGui::Button("Task Manager", buttonSize)) {
        UIManager::getInstance()->toggleWindow("TaskManagerUI");
    }
    ImGui::SameLine();
    
    if (ImGui::Button("BIOS", buttonSize)) {
        UIManager::getInstance()->toggleWindow("BIOSBootWindow");
    }
    ImGui::SameLine();
    
    if (ImGui::Button("Process", buttonSize)) {
        UIManager::getInstance()->toggleWindow("ProcessWindow");
    }
}

void Taskbar::drawSystemTray() {
    auto now = std::chrono::system_clock::now();
    std::time_t now_c = std::chrono::system_clock::to_time_t(now);
    std::tm parts;
    localtime_s(&parts, &now_c);
    
    std::ostringstream timeStream;
    timeStream << std::put_time(&parts, "%H:%M:%S");
    
    ImGui::SameLine(ImGui::GetWindowWidth() - 250);
    ImGui::Text("CPU: 12%%  Mem: 45%%  |  %s", timeStream.str().c_str());
}

void Taskbar::draw() {
    ImGuiIO& io = ImGui::GetIO();
    float taskbarHeight = 50.0f * UIConfig::getInstance()->getScaleFactor();
    
    ImGui::SetNextWindowPos(ImVec2(0, io.DisplaySize.y - taskbarHeight));
    ImGui::SetNextWindowSize(ImVec2(io.DisplaySize.x, taskbarHeight));
    
    ImGuiWindowFlags flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | 
                             ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | 
                             ImGuiWindowFlags_NoSavedSettings;
    
    // We don't use beginWindow() directly here because we need to pass specific flags 
    // to ImGui::Begin that the AWindow wrapper might not support dynamically. 
    // Wait, let's just use ImGui::Begin directly for Taskbar.
    if (isVisible) {
        ImGui::Begin(windowName.c_str(), nullptr, flags);
        
        drawAppIcons();
        drawSystemTray();
        
        ImGui::End();
    }
}
