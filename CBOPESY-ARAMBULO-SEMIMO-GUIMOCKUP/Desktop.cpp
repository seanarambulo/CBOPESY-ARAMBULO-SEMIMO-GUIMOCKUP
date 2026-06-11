#include "Desktop.h"
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>

Desktop::Desktop() : AWindow("Desktop") {
    isVisible = true; // Desktop is always visible
}

void Desktop::drawClock() {
    auto now = std::chrono::system_clock::now();
    std::time_t now_c = std::chrono::system_clock::to_time_t(now);
    std::tm parts;
    localtime_s(&parts, &now_c);
    
    std::ostringstream timeStream;
    timeStream << std::put_time(&parts, "%H:%M:%S");
    
    ImGui::SetCursorPos(ImVec2(ImGui::GetWindowWidth() - 100, 20));
    ImGui::Text("%s", timeStream.str().c_str());
}

void Desktop::draw() {
    ImGuiIO& io = ImGui::GetIO();
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(io.DisplaySize);
    
    ImGuiWindowFlags flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | 
                             ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | 
                             ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
    
    beginWindow();
    
    // Draw desktop background content here if any
    drawClock();
    
    endWindow();
}
