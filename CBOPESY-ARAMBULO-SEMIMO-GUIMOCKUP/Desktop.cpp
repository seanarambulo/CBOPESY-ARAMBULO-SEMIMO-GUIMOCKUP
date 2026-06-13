#include "Desktop.h"
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>

#include "TextureLoader.h"

Desktop::Desktop() : AWindow("Desktop") {
    isVisible = true; // Desktop is always visible
    wallpaperTexture = TextureLoader::loadTexture("bliss.png", texWidth, texHeight);
}

void Desktop::drawClock() {
    auto now = std::chrono::system_clock::now();
    std::time_t now_c = std::chrono::system_clock::to_time_t(now);
    std::tm parts;
    localtime_s(&parts, &now_c);
    
    std::ostringstream timeStream;
    timeStream << std::put_time(&parts, "%A, %b %d, %Y | %I:%M %p");
    std::string timeStr = timeStream.str();
    
    ImGuiIO& io = ImGui::GetIO();
    ImVec2 textSize = ImGui::CalcTextSize(timeStr.c_str());
    ImVec2 pos(io.DisplaySize.x - textSize.x - 20, 10);
    
    // Draw a semi-transparent dark background for the clock
    ImGui::GetWindowDrawList()->AddRectFilled(
        ImVec2(pos.x - 10, pos.y - 5),
        ImVec2(pos.x + textSize.x + 10, pos.y + textSize.y + 5),
        IM_COL32(0, 0, 0, 150)
    );
    
    ImGui::SetCursorPos(pos);
    ImGui::Text("%s", timeStr.c_str());
}

void Desktop::draw() {
    ImGuiIO& io = ImGui::GetIO();
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(io.DisplaySize);
    
    ImGuiWindowFlags flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | 
                             ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | 
                             ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
    
    if (!beginWindow(flags)) return;
    
    // Draw desktop background (gradient or image)
    if (wallpaperTexture != 0) {
        ImGui::GetWindowDrawList()->AddImage(
            (void*)(intptr_t)wallpaperTexture,
            ImVec2(0, 0),
            io.DisplaySize
        );
    } else {
        ImU32 colorTop = IM_COL32(40, 100, 200, 255); // Sky blue
        ImU32 colorBottom = IM_COL32(80, 200, 60, 255); // Grass green
        ImGui::GetWindowDrawList()->AddRectFilledMultiColor(
            ImVec2(0, 0), io.DisplaySize,
            colorTop, colorTop, colorBottom, colorBottom
        );
    }
    
    // Draw OS version text
    ImGui::SetCursorPos(ImVec2(20, io.DisplaySize.y / 2));
    ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "CSOPESY OS v1.0 - System Online");
    
    drawClock();
    
    endWindow();
}
