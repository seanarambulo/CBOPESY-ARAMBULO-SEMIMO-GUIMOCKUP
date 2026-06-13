#pragma once
#include <map>
#include <string>
#include <memory>
#include "AWindow.h"

class UIManager {
public:
    static UIManager& getInstance();

    void initialize();
    void exitApplication();
    bool isApplicationClosing() const;

    void registerWindow(const std::string& name, std::shared_ptr<AWindow> window);
    void showWindow(const std::string& name);
    void hideWindow(const std::string& name);
    void toggleWindow(const std::string& name);

    std::shared_ptr<AWindow> getWindow(const std::string& name);

    void updateAllWindows();
    void renderAllWindows();

private:
    UIManager(); // Private constructor
    std::map<std::string, std::shared_ptr<AWindow>> windows;
    bool bApplicationShouldClose;
};
