#pragma once
#include <map>
#include <string>
#include <memory>
#include "AWindow.h"

class UIManager {
private:
    static UIManager* instance;
    std::map<std::string, std::shared_ptr<AWindow>> windows;

    UIManager() {}

public:
    static UIManager* getInstance() {
        if (instance == nullptr) {
            instance = new UIManager();
        }
        return instance;
    }

    void registerWindow(std::shared_ptr<AWindow> window) {
        windows[window->getName()] = window;
    }

    void showWindow(const std::string& name) {
        if (windows.find(name) != windows.end()) {
            windows[name]->setVisibility(true);
        }
    }

    void hideWindow(const std::string& name) {
        if (windows.find(name) != windows.end()) {
            windows[name]->setVisibility(false);
        }
    }

    void toggleWindow(const std::string& name) {
        if (windows.find(name) != windows.end()) {
            windows[name]->setVisibility(!windows[name]->getVisibility());
        }
    }

    void renderAllWindows() {
        for (auto const& [name, window] : windows) {
            if (window->getVisibility()) {
                window->draw();
            }
        }
    }

    std::shared_ptr<AWindow> getWindow(const std::string& name) {
        if (windows.find(name) != windows.end()) {
            return windows[name];
        }
        return nullptr;
    }
};
