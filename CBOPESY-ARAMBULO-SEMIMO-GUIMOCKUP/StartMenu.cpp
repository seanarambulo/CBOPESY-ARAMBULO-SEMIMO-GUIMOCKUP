#include "StartMenu.h"
#include "UIConfig.h"
#include "UIManager.h"
#include "imgui.h"

StartMenu::StartMenu() : AWindow("StartMenu") {
    isVisible = false;
}

void StartMenu::draw() {
    if (!isVisible) return;

    ImGuiIO& io = ImGui::GetIO();
    float scale = UIConfig::getScaleFactor();
    float taskbarHeight = 60.0f * scale;
    float startMenuWidth = 400.0f * scale;
    float startMenuHeight = 500.0f * scale;

    ImGui::SetNextWindowPos(ImVec2(0, io.DisplaySize.y - taskbarHeight - startMenuHeight), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(startMenuWidth, startMenuHeight), ImGuiCond_Always);

    ImGuiWindowFlags flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | 
                             ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | 
                             ImGuiWindowFlags_NoSavedSettings;

    if (ImGui::Begin("StartMenuWindow", nullptr, flags)) {
        ImGui::Columns(2, "StartMenuColumns", false);
        ImGui::SetColumnWidth(0, startMenuWidth * 0.6f);

        // Left Pane: Apps
        ImGui::Text("Recent Apps");
        ImGui::Separator();
        
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0)); // Transparent buttons
        ImGui::PushStyleVar(ImGuiStyleVar_ButtonTextAlign, ImVec2(0.0f, 0.5f)); // Left align text
        ImGui::Button("Command Prompt", ImVec2(-1, 0));
        ImGui::Button("Calculator", ImVec2(-1, 0));
        ImGui::Button("Notepad", ImVec2(-1, 0));
        ImGui::Button("Paint", ImVec2(-1, 0));
        ImGui::Button("Snipping Tool", ImVec2(-1, 0));
        ImGui::Button("WordPad", ImVec2(-1, 0));
        ImGui::PopStyleVar();
        ImGui::PopStyleColor();

        // Push to bottom for Search bar
        ImGui::SetCursorPosY(startMenuHeight - 40 * scale);
        static char searchBuffer[128] = "";
        ImGui::SetNextItemWidth(-1);
        ImGui::InputTextWithHint("##Search", "Search programs and files", searchBuffer, IM_ARRAYSIZE(searchBuffer));

        ImGui::NextColumn();

        // Right Pane: Folders & System
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
        ImGui::PushStyleVar(ImGuiStyleVar_ButtonTextAlign, ImVec2(0.0f, 0.5f));
        ImGui::Text("User");
        ImGui::Separator();
        ImGui::Button("Documents", ImVec2(-1, 0));
        ImGui::Button("Pictures", ImVec2(-1, 0));
        ImGui::Button("Music", ImVec2(-1, 0));
        ImGui::Separator();
        ImGui::Button("Computer", ImVec2(-1, 0));
        ImGui::Button("Control Panel", ImVec2(-1, 0));
        ImGui::Button("Devices and Printers", ImVec2(-1, 0));
        ImGui::Button("Help and Support", ImVec2(-1, 0));
        ImGui::PopStyleVar();
        ImGui::PopStyleColor();

        // Push to bottom for Shut down
        ImGui::SetCursorPosY(startMenuHeight - 40 * scale);
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.8f, 0.1f, 0.1f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(1.0f, 0.2f, 0.2f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.6f, 0.0f, 0.0f, 1.0f));
        if (ImGui::Button("Shut down", ImVec2(-1, 0))) {
            UIManager::getInstance().exitApplication();
        }
        ImGui::PopStyleColor(3);

        ImGui::Columns(1);
    }
    ImGui::End();
}
