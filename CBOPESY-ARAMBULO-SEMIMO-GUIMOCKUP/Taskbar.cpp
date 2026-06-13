#include "Taskbar.h"
#include "UIConfig.h"
#include "UIManager.h"
#include "TextureLoader.h"
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>

Taskbar::Taskbar() : AWindow("Taskbar") {
    isVisible = true; // Taskbar is always visible

    int w, h;
    folderIcon = TextureLoader::loadTexture("folder.png", w, h);
    initIcon = TextureLoader::loadTexture("init.png", w, h);
    startIcon = TextureLoader::loadTexture("Start-Button-Vector-PNG-Images.png", w, h);
    stopIcon = TextureLoader::loadTexture("stop.png", w, h);
    chartIcon = TextureLoader::loadTexture("chart.png", w, h);
    searchIcon = TextureLoader::loadTexture("search.png", w, h);
}

void Taskbar::drawAppIcons() {
    float scale = UIConfig::getScaleFactor();
    ImVec2 buttonSize(35 * scale, 35 * scale); // Square for icons
    
    auto DrawIconBtn = [&](const char* id, unsigned int tex, const char* text, ImVec2 size = ImVec2(0, 0)) {
        bool clicked = false;
        ImVec2 actualSize = (size.x == 0 && size.y == 0) ? buttonSize : size;
        if (tex != 0) {
#if IMGUI_VERSION_NUM >= 18900
            clicked = ImGui::ImageButton(id, (ImTextureID)(intptr_t)tex, actualSize);
#else
            clicked = ImGui::ImageButton((ImTextureID)(intptr_t)tex, actualSize);
#endif
        } else {
            clicked = ImGui::Button(text, ImVec2(80 * scale, 30 * scale));
        }
        return clicked;
    };

    if (DrawIconBtn("btn_start", startIcon, "START", ImVec2(100 * scale, 35 * scale))) {
        UIManager::getInstance().toggleWindow("startMenu");
    }
    ImGui::SameLine();
    
    if (DrawIconBtn("btn_folder", folderIcon, "Folder")) {
        showFolderDummy = true;
    }
    ImGui::SameLine();
    
    if (DrawIconBtn("btn_init", initIcon, "INIT")) {
        showInitDummy = true;
    }
    ImGui::SameLine();
    
    if (DrawIconBtn("btn_stop", stopIcon, "STOP")) {
        showStopDummy = true;
    }
    ImGui::SameLine();

    if (DrawIconBtn("btn_chart", chartIcon, "Chart")) {
        UIManager::getInstance().toggleWindow("taskManager");
    }
    ImGui::SameLine();

    if (DrawIconBtn("btn_search", searchIcon, "SRCH")) {
        UIManager::getInstance().toggleWindow("search");
    }
}

void Taskbar::drawSystemTray() {
    float scale = UIConfig::getScaleFactor();
    ImVec2 buttonSize(50 * scale, 30 * scale);
    
    // Right align the tray icons
    ImGui::SameLine(ImGui::GetWindowWidth() - 180 * scale);
    
    ImGui::Button("VOL", buttonSize);
    ImGui::SameLine();
    ImGui::Button("NET", buttonSize);
    ImGui::SameLine();
    
    // Red PWR button
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.8f, 0.1f, 0.1f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(1.0f, 0.2f, 0.2f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.6f, 0.0f, 0.0f, 1.0f));
    
    if (ImGui::Button("PWR", buttonSize)) {
        UIManager::getInstance().exitApplication();
    }
    
    ImGui::PopStyleColor(3);
}

void Taskbar::draw() {
    ImGuiIO& io = ImGui::GetIO();
    float taskbarHeight = 60.0f * UIConfig::getScaleFactor();
    
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
        
        ImGui::End();
    }

    if (showFolderDummy) {
        ImGui::SetNextWindowSize(ImVec2(400, 150), ImGuiCond_FirstUseEver);
        if (ImGui::Begin("File Explorer", &showFolderDummy)) {
            ImGui::Text("File system functionality is currently under construction.\nCheck back later!");
            ImGui::Spacing();
            if (ImGui::Button("Close")) showFolderDummy = false;
        }
        ImGui::End();
    }

    if (showInitDummy) {
        ImGui::SetNextWindowSize(ImVec2(400, 150), ImGuiCond_FirstUseEver);
        if (ImGui::Begin("Initialize Mode", &showInitDummy)) {
            ImGui::Text("System initialization tools are not yet available.");
            ImGui::Spacing();
            if (ImGui::Button("Close")) showInitDummy = false;
        }
        ImGui::End();
    }

    if (showStopDummy) {
        ImGui::SetNextWindowSize(ImVec2(400, 150), ImGuiCond_FirstUseEver);
        if (ImGui::Begin("Emergency Stop", &showStopDummy)) {
            ImGui::Text("Stop sequence is unavailable in mockup mode.");
            ImGui::Spacing();
            if (ImGui::Button("Close")) showStopDummy = false;
        }
        ImGui::End();
    }
}
