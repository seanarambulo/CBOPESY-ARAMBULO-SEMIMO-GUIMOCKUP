#include "ProcessWindow.h"

ProcessWindow::ProcessWindow() : AWindow("ProcessWindow"), 
    processName("explorer.exe"), pid(2560), cpuUsage(5.0f), memoryUsage(45.2f), 
    isPaused(false), currentPriority(1) {
}

void ProcessWindow::draw() {
    beginWindow();
    
    ImGui::Text("Process Details");
    ImGui::Separator();
    
    ImGui::Text("Name: %s", processName.c_str());
    ImGui::Text("PID: %d", pid);
    ImGui::Text("CPU Usage: %.1f%%", cpuUsage);
    ImGui::Text("Memory Usage: %.1f MB", memoryUsage);
    
    ImGui::Spacing();
    
    if (ImGui::Button(isPaused ? "Resume" : "Pause")) {
        isPaused = !isPaused;
    }
    
    ImGui::SameLine();
    
    if (ImGui::Button("Kill Process")) {
        // Mock kill functionality
        isVisible = false;
    }
    
    ImGui::Spacing();
    
    const char* priorities[] = { "Low", "Normal", "High" };
    ImGui::Combo("Priority", &currentPriority, priorities, IM_ARRAYSIZE(priorities));
    
    endWindow();
}
