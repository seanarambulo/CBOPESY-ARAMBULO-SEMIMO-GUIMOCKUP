#include "BIOSBootWindow.h"
#include "UIManager.h"
#include "imgui.h"

BIOSBootWindow::BIOSBootWindow() : AWindow("BIOS") {
    isVisible = true;
    startTime = std::chrono::system_clock::now();
    ramCounter = 0;
}

void BIOSBootWindow::draw() {
    auto now = std::chrono::system_clock::now();
    std::chrono::duration<float> elapsed = now - startTime;
    float elapsedSeconds = elapsed.count();

    ImGuiIO& io = ImGui::GetIO();
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(io.DisplaySize);

    ImGuiWindowFlags flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | 
                             ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | 
                             ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0, 0, 0, 1)); // Black background

    if (ImGui::Begin("BIOS", nullptr, flags)) {
        if (elapsedSeconds < 8.0f) {
            // Part 1: AMI Megatrends POST Screen
            ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[0]);
            
            // Logo (Big C in Cyan/Blue)
            ImVec4 archBlue = ImVec4(0.09f, 0.57f, 0.82f, 1.0f);
            ImVec4 white = ImVec4(0.9f, 0.9f, 0.9f, 1.0f);
            
            ImGui::TextColored(archBlue, "  ______");
            ImGui::SameLine(100); ImGui::TextColored(white, "CSOPESY");
            ImGui::TextColored(archBlue, " /      \\");
            ImGui::SameLine(100); ImGui::TextColored(white, "Megatrends");
            ImGui::TextColored(archBlue, "|  /----'");
            ImGui::TextColored(archBlue, "|  |");
            ImGui::TextColored(archBlue, "|  \\----.");
            ImGui::TextColored(archBlue, " \\______/");
            ImGui::Spacing();
            ImGui::Spacing();

            if (elapsedSeconds > 1.0f) {
                ImGui::TextColored(white, "OPESYBIOS(C)2021 CSOPESY Megatrends, Inc.");
                ImGui::Spacing();
            }
            if (elapsedSeconds > 1.5f) {
                ImGui::TextColored(white, "CSOPESY Quantum Board Revision 9001");
                ImGui::TextColored(white, "CPU: CSOPESY Custom Silicon v1 (128 Cores) @ 3.6 GHz");
                ImGui::TextColored(white, "Speed: 9600MHz");
                ImGui::Spacing();
            }
            if (elapsedSeconds > 2.5f) {
                // Memory counting up to 65536
                int memCount = (int)(65536 * ((elapsedSeconds - 2.5f) / 1.0f));
                if (memCount > 65536) memCount = 65536;
                ImGui::TextColored(white, "Total Memory: %dMB (DDR6-12800)", memCount);
                ImGui::Spacing();
            }
            if (elapsedSeconds > 4.0f) {
                ImGui::TextColored(white, "USB Devices total: 0 Drive, 1 Neural Link, 1 Mouse, 0 Hub");
                ImGui::Spacing();
                ImGui::TextColored(white, "Detected Storage Devices...");
                ImGui::TextColored(white, "NVMe_1: QUANTUM TARDIS 50TB");
                ImGui::TextColored(white, "NVMe_2: SEAGATE FIRECUDA 4TB");
                ImGui::TextColored(white, "M.2: CORTEX BRAIN CHIP V3");
                ImGui::Spacing();
                ImGui::Spacing();
            }
            if (elapsedSeconds > 5.5f) {
                ImGui::TextColored(white, "SMART Failure Predicted on M.2 : CORTEX BRAIN CHIP V3");
                ImGui::TextColored(white, "WARNING: Please back-up your memories and replace your brain chip.");
                ImGui::TextColored(white, "A failure may be imminent and cause cyber-psychosis.");
                ImGui::Spacing();
            }
            if (elapsedSeconds > 6.5f) {
                ImGui::TextColored(white, "Press F1 to Run SETUP");
            }
            
            ImGui::PopFont();
        } else if (elapsedSeconds < 27.0f) {
            // Part 2: OS Loading Screen with Lyrics
            ImVec2 windowSize = ImGui::GetWindowSize();
            const char* logo = 
                "  ____  ____   ___  ____  _____  _____ __  __ \n"
                " / ___|/ ___| / _ \\|  _ \\| ____|/ ___|\\ \\/ / \n"
                "| |    \\___ \\| | | | |_) |  _|  \\___ \\ \\  /  \n"
                "| |___  ___) | |_| |  __/| |___  ___) |/  \\  \n"
                " \\____||____/ \\___/|_|   |_____||____//_/\\_\\ \n";
            
            ImVec2 textSize = ImGui::CalcTextSize(logo);
            ImGui::SetCursorPos(ImVec2((windowSize.x - textSize.x) / 2.0f, windowSize.y / 6.0f));
            ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 1.0f), "%s", logo);
            
            const char* subtitle = "CSOPESY Operating System Emulator v1.0";
            ImVec2 subSize = ImGui::CalcTextSize(subtitle);
            ImGui::SetCursorPos(ImVec2((windowSize.x - subSize.x) / 2.0f, windowSize.y / 6.0f + textSize.y + 20.0f));
            ImGui::TextColored(ImVec4(0.8f, 0.8f, 0.8f, 1.0f), "%s", subtitle);

            const char* subtitle2 = "Created by Sean Marthy Arambulo";
            ImVec2 subSize2 = ImGui::CalcTextSize(subtitle2);
            ImGui::SetCursorPos(ImVec2((windowSize.x - subSize2.x) / 2.0f, windowSize.y / 6.0f + textSize.y + 40.0f));
            ImGui::TextColored(ImVec4(0.8f, 0.8f, 0.8f, 1.0f), "%s", subtitle2);

            // Lyrics boot sequence
            const char* lyrics[] = {
                "Loading kernel modules...",
                "Initializing network interface...",
                "[OK] Hindi mapigil ang tibok ng aking puso",
                "[OK] Sa t'wing ako'y nakatingin sa'yo",
                "[OK] Maaari bang 'wag kang humiwalay?",
                "[OK] Dahil sandali na lang",
                "[OK] Darating din ang gabing walang pipigil sa'tin",
                "[OK] Kung hindi ngayon, aasa bang maibabalik ang kahapon?",
                "[WARNING] Kahit sandali",
                "[WARNING] Palayain ang pusong 'di mapigil",
                "[WARNING] Sana'y tayong dalawa sa huling pagkakataon",
                "[WARNING] Na hindi na para sa'tin",
                "[ERROR] At sa bawat minuto, ako'y 'di natuto",
                "[ERROR] Ipilit mang iba, ako'y maghihintay sa'yo",
                "[ERROR] Ikaw ang aking kapiling sa huling sandali",
                "[FATAL] Kasalanan ba kung puso natin ang magwawagi?",
                "Ignoring fatal error... continuing boot..."
            };
            int numLyrics = sizeof(lyrics) / sizeof(lyrics[0]);
            
            // Show one line every 1 second after the first 8 seconds
            int linesToShow = (int)(elapsedSeconds - 8.0f);
            if (linesToShow > numLyrics) linesToShow = numLyrics;

            ImGui::SetCursorPos(ImVec2(50.0f, windowSize.y / 6.0f + textSize.y + 80.0f));
            for (int i = 0; i < linesToShow; ++i) {
                ImVec4 color = ImVec4(0.0f, 1.0f, 0.0f, 1.0f); // Default green
                if (strstr(lyrics[i], "[WARNING]")) color = ImVec4(1.0f, 1.0f, 0.0f, 1.0f); // Yellow
                if (strstr(lyrics[i], "[ERROR]") || strstr(lyrics[i], "[FATAL]")) color = ImVec4(1.0f, 0.0f, 0.0f, 1.0f); // Red
                ImGui::TextColored(color, "%s", lyrics[i]);
                ImGui::SetCursorPosX(50.0f); // Reset X for next line
            }

            // Green "Loading." at the bottom
            int numDots = (int)(elapsedSeconds * 5) % 10;
            std::string loadingStr = "Booting OS";
            for (int i = 0; i < numDots; ++i) loadingStr += ".";
            ImGui::SetCursorPos(ImVec2(50.0f, windowSize.y - 50.0f));
            ImGui::TextColored(ImVec4(0.5f, 0.5f, 0.5f, 1.0f), "%s", loadingStr.c_str());
            
        } else {
            // Transition to Desktop
            UIManager::getInstance().hideWindow("bios");
            UIManager::getInstance().showWindow("desktop");
            UIManager::getInstance().showWindow("taskbar");
        }
    }
    ImGui::End();
    ImGui::PopStyleColor();
}
