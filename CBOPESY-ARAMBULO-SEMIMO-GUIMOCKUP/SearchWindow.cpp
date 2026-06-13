#include "SearchWindow.h"
#include "imgui.h"

SearchWindow::SearchWindow() : AWindow("Search") {
}

void SearchWindow::draw() {
    if (!beginWindow()) return;
    
    ImGui::Text("System Search Placeholder");
    ImGui::Separator();
    
    static char buf[128] = "";
    ImGui::InputText("Query", buf, IM_ARRAYSIZE(buf));
    
    if (ImGui::Button("Search")) {
        // Do nothing for now
    }
    
    endWindow();
}
