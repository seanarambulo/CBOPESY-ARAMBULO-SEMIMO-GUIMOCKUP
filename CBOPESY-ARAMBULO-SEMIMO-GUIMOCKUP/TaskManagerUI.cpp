#include "TaskManagerUI.h"
#include <algorithm>
#include <cstdlib>

enum class TableType {
    RUNNING,
    WAITING,
    PROCESSES
};

static bool compareDummyProcesses(const DummyProcess& a, const DummyProcess& b, const ImGuiTableColumnSortSpecs* sort_spec, TableType type) {
    if (type == TableType::RUNNING) {
        switch (sort_spec->ColumnIndex) {
            case 0: return a.pid < b.pid;
            case 1: return a.name < b.name;
            case 2: return a.core < b.core;
            case 3: return ((float)a.currentLine/a.totalLines) < ((float)b.currentLine/b.totalLines);
            case 4: return a.currentLine < b.currentLine;
            default: return a.pid < b.pid;
        }
    } else if (type == TableType::WAITING) {
        switch (sort_spec->ColumnIndex) {
            case 0: return a.pid < b.pid;
            case 1: return a.name < b.name;
            case 2: return a.state < b.state;
            default: return a.pid < b.pid;
        }
    } else { // PROCESSES
        switch (sort_spec->ColumnIndex) {
            case 0: return a.pid < b.pid;
            case 1: return a.name < b.name;
            case 2: return a.state < b.state;
            case 3: return a.cpuUsage < b.cpuUsage;
            case 4: return a.memUsage < b.memUsage;
            default: return a.pid < b.pid;
        }
    }
}

static void sortProcesses(std::vector<DummyProcess>& processes, ImGuiTableSortSpecs* sorts_specs, TableType type) {
    if (!sorts_specs || !sorts_specs->SpecsDirty || sorts_specs->SpecsCount == 0) return;

    const ImGuiTableColumnSortSpecs* sort_spec = &sorts_specs->Specs[0];
    
    std::sort(processes.begin(), processes.end(), [sort_spec, type](const DummyProcess& a, const DummyProcess& b) {
        if (sort_spec->SortDirection == ImGuiSortDirection_Ascending) {
            return compareDummyProcesses(a, b, sort_spec, type);
        } else {
            return compareDummyProcesses(b, a, sort_spec, type);
        }
    });
    sorts_specs->SpecsDirty = false;
}

TaskManagerUI::TaskManagerUI() : AWindow("CSOPESY Task Manager") {
    cpuHistory.resize(100, 0.0f);
    memoryHistory.resize(100, 0.0f);
    
    dummyProcesses = {
        {1, "System Idle Process", ProcessState::RUNNING, 85.0f, 0.1f, 0, 100, 100},
        {4, "System", ProcessState::RUNNING, 2.0f, 0.5f, 1, 40, 150},
        {128, "Registry", ProcessState::WAITING, 0.0f, 1.2f, 2, 20, 50},
        {544, "smss.exe", ProcessState::READY, 0.0f, 0.3f, 3, 5, 20},
        {800, "csrss.exe", ProcessState::RUNNING, 1.5f, 2.4f, 4, 30, 200},
        {924, "wininit.exe", ProcessState::WAITING, 0.0f, 0.8f, 5, 10, 80},
        {1020, "services.exe", ProcessState::RUNNING, 3.2f, 8.5f, 6, 27, 157},
        {1044, "lsass.exe", ProcessState::READY, 0.5f, 4.2f, 7, 0, 100},
        {1308, "svchost.exe", ProcessState::RUNNING, 2.8f, 15.6f, 8, 45, 120},
        {2560, "explorer.exe", ProcessState::RUNNING, 5.0f, 45.2f, 9, 80, 200},
        {9999, "terminated_proc", ProcessState::TERMINATED, 0.0f, 0.0f, 10, 100, 100}
    };
}

void TaskManagerUI::updatePerformanceData() {
    std::rotate(cpuHistory.begin(), cpuHistory.begin() + 1, cpuHistory.end());
    std::rotate(memoryHistory.begin(), memoryHistory.begin() + 1, memoryHistory.end());
    
    cpuHistory.back() = 10.0f + (std::rand() % 20); 
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

void TaskManagerUI::drawHardwareHeader() {
    float cpuUsage = cpuHistory.back();
    ImGui::Text("CPU Utilization: ");
    ImGui::SameLine();
    ImGui::ProgressBar(cpuUsage / 100.0f, ImVec2(-1.0f, 0.0f));
    ImGui::Text("Cores Used: 6   Cores Available: 122   Total Cores: 128");
    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();
}

void TaskManagerUI::drawPerformanceTab() {
    updatePerformanceData();
    ImGui::Text("CPU Usage");
    ImGui::PlotLines("##CPU", cpuHistory.data(), cpuHistory.size(), 0, nullptr, 0.0f, 100.0f, ImVec2(0, 80));
    ImGui::Spacing();
    ImGui::Text("Memory Usage");
    ImGui::PlotLines("##Memory", memoryHistory.data(), memoryHistory.size(), 0, nullptr, 0.0f, 100.0f, ImVec2(0, 80));
}

void TaskManagerUI::drawRunningTab() {
    if (ImGui::BeginTable("RunningTable", 5, ImGuiTableFlags_Resizable | ImGuiTableFlags_Sortable | ImGuiTableFlags_Borders)) {
        ImGui::TableSetupColumn("PID");
        ImGui::TableSetupColumn("Name");
        ImGui::TableSetupColumn("Core");
        ImGui::TableSetupColumn("Progress");
        ImGui::TableSetupColumn("Lines");
        ImGui::TableHeadersRow();
        
        sortProcesses(dummyProcesses, ImGui::TableGetSortSpecs(), TableType::RUNNING);

        for (const auto& process : dummyProcesses) {
            if (process.state != ProcessState::RUNNING) continue;
            
            ImGui::TableNextRow();
            
            ImGui::TableSetColumnIndex(0);
            ImGui::Text("%d", process.pid);
            
            ImGui::TableSetColumnIndex(1);
            ImGui::Text("%s", process.name.c_str());
            
            ImGui::TableSetColumnIndex(2);
            ImGui::Text("Core %d", process.core);
            
            ImGui::TableSetColumnIndex(3);
            float progress = (float)process.currentLine / (float)process.totalLines;
            ImGui::PushStyleColor(ImGuiCol_PlotHistogram, ImVec4(0.8f, 0.6f, 0.1f, 1.0f)); // Orange/Yellow
            char buf[32];
            snprintf(buf, sizeof(buf), "%d%%", (int)(progress * 100));
            ImGui::ProgressBar(progress, ImVec2(-1.0f, 0.0f), buf);
            ImGui::PopStyleColor();
            
            ImGui::TableSetColumnIndex(4);
            ImGui::Text("%d / %d", process.currentLine, process.totalLines);
        }
        ImGui::EndTable();
    }
}

void TaskManagerUI::drawWaitingTab() {
    ImGui::Text("Processes waiting for I/O or resources:");
    if (ImGui::BeginTable("WaitingTable", 3, ImGuiTableFlags_Resizable | ImGuiTableFlags_Sortable | ImGuiTableFlags_Borders)) {
        ImGui::TableSetupColumn("PID");
        ImGui::TableSetupColumn("Name");
        ImGui::TableSetupColumn("Status");
        ImGui::TableHeadersRow();

        sortProcesses(dummyProcesses, ImGui::TableGetSortSpecs(), TableType::WAITING);

        for (const auto& process : dummyProcesses) {
            if (process.state != ProcessState::WAITING) continue;
            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0); ImGui::Text("%d", process.pid);
            ImGui::TableSetColumnIndex(1); ImGui::Text("%s", process.name.c_str());
            ImGui::TableSetColumnIndex(2); ImGui::TextColored(getStateColor(process.state), "Waiting");
        }
        ImGui::EndTable();
    }
}

void TaskManagerUI::drawFinishedTab() {
    ImGui::Text("Terminated Processes:");
    if (ImGui::BeginTable("FinishedTable", 3, ImGuiTableFlags_Resizable | ImGuiTableFlags_Sortable | ImGuiTableFlags_Borders)) {
        ImGui::TableSetupColumn("PID");
        ImGui::TableSetupColumn("Name");
        ImGui::TableSetupColumn("Status");
        ImGui::TableHeadersRow();

        sortProcesses(dummyProcesses, ImGui::TableGetSortSpecs(), TableType::WAITING);

        for (const auto& process : dummyProcesses) {
            if (process.state != ProcessState::TERMINATED) continue;
            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0); ImGui::Text("%d", process.pid);
            ImGui::TableSetColumnIndex(1); ImGui::Text("%s", process.name.c_str());
            ImGui::TableSetColumnIndex(2); ImGui::TextColored(getStateColor(process.state), "Finished");
        }
        ImGui::EndTable();
    }
}

void TaskManagerUI::drawProcessesTab() {
    if (ImGui::BeginTable("ProcessesTable", 5, ImGuiTableFlags_Resizable | ImGuiTableFlags_Sortable | ImGuiTableFlags_Borders)) {
        ImGui::TableSetupColumn("PID");
        ImGui::TableSetupColumn("Name");
        ImGui::TableSetupColumn("State");
        ImGui::TableSetupColumn("CPU (%)");
        ImGui::TableSetupColumn("Memory (MB)");
        ImGui::TableHeadersRow();
        
        sortProcesses(dummyProcesses, ImGui::TableGetSortSpecs(), TableType::PROCESSES);

        for (const auto& process : dummyProcesses) {
            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0); ImGui::Text("%d", process.pid);
            ImGui::TableSetColumnIndex(1); ImGui::Text("%s", process.name.c_str());
            ImGui::TableSetColumnIndex(2); 
            std::string stateStr;
            switch(process.state) {
                case ProcessState::RUNNING: stateStr = "Running"; break;
                case ProcessState::READY: stateStr = "Ready"; break;
                case ProcessState::WAITING: stateStr = "Waiting"; break;
                case ProcessState::TERMINATED: stateStr = "Terminated"; break;
            }
            ImGui::TextColored(getStateColor(process.state), "%s", stateStr.c_str());
            ImGui::TableSetColumnIndex(3); ImGui::Text("%.1f", process.cpuUsage);
            ImGui::TableSetColumnIndex(4); ImGui::Text("%.1f", process.memUsage);
        }
        ImGui::EndTable();
    }
}

void TaskManagerUI::draw() {
    if (!beginWindow()) return;
    
    drawHardwareHeader();
    
    if (ImGui::BeginTabBar("TaskManagerTabs")) {
        if (ImGui::BeginTabItem("Performance")) {
            drawPerformanceTab();
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Running")) {
            drawRunningTab();
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Waiting")) {
            drawWaitingTab();
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Finished")) {
            drawFinishedTab();
            ImGui::EndTabItem();
        }
        ImGui::EndTabBar();
    }
    
    endWindow();
}
