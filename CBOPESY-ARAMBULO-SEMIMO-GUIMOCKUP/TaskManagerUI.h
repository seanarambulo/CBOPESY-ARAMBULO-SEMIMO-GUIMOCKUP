#pragma once
#include "AWindow.h"
#include <vector>
#include <string>

enum class ProcessState {
    RUNNING,
    READY,
    WAITING,
    TERMINATED
};

struct DummyProcess {
    int pid;
    std::string name;
    ProcessState state;
    float cpuUsage;
    float memUsage;
};

class TaskManagerUI : public AWindow {
public:
    TaskManagerUI();
    void draw() override;

private:
    void drawPerformanceTab();
    void drawProcessesTab();
    void updatePerformanceData();
    ImVec4 getStateColor(ProcessState state);

    std::vector<float> cpuHistory;
    std::vector<float> memoryHistory;
    std::vector<DummyProcess> dummyProcesses;
};
