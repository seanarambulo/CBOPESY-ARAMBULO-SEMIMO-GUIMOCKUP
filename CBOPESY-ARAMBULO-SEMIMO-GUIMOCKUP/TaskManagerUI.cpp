#include "TaskManagerUI.h"
#include <algorithm>
#include <cstdlib>

TaskManagerUI::TaskManagerUI() : AWindow("TaskManagerUI") {
    cpuHistory.resize(100, 0.0f);
    memoryHistory.resize(100, 0.0f);
    
    dummyProcesses = {
        {1, "System Idle Process", ProcessState::RUNNING, 85.0f, 0.1f},
        {4, "System", ProcessState::RUNNING, 2.0f, 0.5f},
        {128, "Registry", ProcessState::WAITING, 0.0f, 1.2f},
        {544, "smss.exe", ProcessState::READY, 0.0f, 0.3f},
        {800, "csrss.exe", ProcessState::RUNNING, 1.5f, 2.4f},
        {924, "wininit.exe", ProcessState::WAITING, 0.0f, 0.8f},
        {1020, "services.exe", ProcessState::RUNNING, 3.2f, 8.5f},
        {1044, "lsass.exe", ProcessState::READY, 0.5f, 4.2f},
        {1308, "svchost.exe", ProcessState::RUNNING, 2.8f, 15.6f},
        {2560, "explorer.exe", ProcessState::RUNNING, 5.0f, 45.2f}
    };
}

void TaskManagerUI::updatePerformanceData() {
    // Shift values and add a new random value
    std::rotate(cpuHistory.begin(), cpuHistory.begin() + 1, cpuHistory.end());
    std::rotate(memoryHistory.begin(), memoryHistory.begin() + 1, memoryHistory.end());
    
    cpuHistory.back() = 10.0f + (std::rand() % 20); // Dummy fluctuation
    memoryHistory.back() = 40.0f + (std::rand() % 10);
}

ImVec4 TaskManagerUI::getStateColor(ProcessState state) {
    switch(state) {
        case ProcessState::RUNNING: return ImVec4(0.2f, 0.8f, 0.2f, 1.0f); // Green
        case ProcessState::READY: return ImVec4(0.8f, 0.8f, 0.2f, 1.0f); // Yellow
        case ProcessState::WAITING: return ImVec4(0.8f, 0.4f, 0.0f, 1.0f); // Orange
        case ProcessState::TERMINATED: return ImVec4(0.8f, 0.2f, 0.2f, 1.0f); // Red
        default: return ImVec4(1.0f, 1.0f, 1.0f, 1.0f); // White
    }
}

void TaskManagerUI::drawPerformanceTab() {
    updatePerformanceData();
    
    ImGui::Text("CPU Usage");
    ImGui::PlotLines("##CPU", cpuHistory.data(), cpuHistory.size(), 0, nullptr, 0.0f, 100.0f, ImVec2(0, 80));
    
    ImGui::Spacing();
    
    ImGui::Text("Memory Usage");
    ImGui::PlotLines("##Memory", memoryHistory.data(), memoryHistory.size(), 0, nullptr, 0.0f, 100.0f, ImVec2(0, 80));
}

void TaskManagerUI::drawProcessesTab() {
    if (ImGui::BeginTable("ProcessesTable", 5, ImGuiTableFlags_Resizable | ImGuiTableFlags_Sortable | ImGuiTableFlags_Borders)) {
        ImGui::TableSetupColumn("PID");
        ImGui::TableSetupColumn("Name");
        ImGui::TableSetupColumn("State");
        ImGui::TableSetupColumn("CPU (%)");
        ImGui::TableSetupColumn("Memory (MB)");
        ImGui::TableHeadersRow();
        
        for (const auto& process : dummyProcesses) {
            ImGui::TableNextRow();
            
            ImGui::TableSetColumnIndex(0);
            ImGui::Text("%d", process.pid);
            
            ImGui::TableSetColumnIndex(1);
            ImGui::Text("%s", process.name.c_str());
            
            ImGui::TableSetColumnIndex(2);
            std::string stateStr;
            switch(process.state) {
                case ProcessState::RUNNING: stateStr = "Running"; break;
                case ProcessState::READY: stateStr = "Ready"; break;
                case ProcessState::WAITING: stateStr = "Waiting"; break;
                case ProcessState::TERMINATED: stateStr = "Terminated"; break;
            }
            ImGui::TextColored(getStateColor(process.state), "%s", stateStr.c_str());
            
            ImGui::TableSetColumnIndex(3);
            ImGui::Text("%.1f", process.cpuUsage);
            
            ImGui::TableSetColumnIndex(4);
            ImGui::Text("%.1f", process.memUsage);
        }
        
        ImGui::EndTable();
    }
}

void TaskManagerUI::draw() {
    beginWindow();
    
    if (ImGui::BeginTabBar("TaskManagerTabs")) {
        if (ImGui::BeginTabItem("Processes")) {
            drawProcessesTab();
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Performance")) {
            drawPerformanceTab();
            ImGui::EndTabItem();
        }
        ImGui::EndTabBar();
    }
    
    endWindow();
}
