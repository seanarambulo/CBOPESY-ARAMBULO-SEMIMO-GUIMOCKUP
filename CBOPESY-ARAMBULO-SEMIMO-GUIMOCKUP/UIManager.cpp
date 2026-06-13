#include "UIManager.h"

UIManager& UIManager::getInstance() {
    static UIManager instance;
    return instance;
}

UIManager::UIManager() : bApplicationShouldClose(false) {}

void UIManager::initialize() {
    bApplicationShouldClose = false;
}

void UIManager::exitApplication() {
    bApplicationShouldClose = true;
}

bool UIManager::isApplicationClosing() const {
    return bApplicationShouldClose;
}

void UIManager::registerWindow(const std::string& name, std::shared_ptr<AWindow> window) {
    windows[name] = window;
}

void UIManager::showWindow(const std::string& name) {
    if (windows.find(name) != windows.end()) {
        windows[name]->show();
    }
}

void UIManager::hideWindow(const std::string& name) {
    if (windows.find(name) != windows.end()) {
        windows[name]->hide();
    }
}

void UIManager::toggleWindow(const std::string& name) {
    if (windows.find(name) != windows.end()) {
        if (windows[name]->isShown()) {
            windows[name]->hide();
        } else {
            windows[name]->show();
        }
    }
}

std::shared_ptr<AWindow> UIManager::getWindow(const std::string& name) {
    if (windows.find(name) != windows.end()) {
        return windows[name];
    }
    return nullptr;
}

void UIManager::updateAllWindows() {
    for (auto& [name, window] : windows) {
        if (window->isShown()) {
            // window->update(); // Update logic (if needed)
        }
    }
}

void UIManager::renderAllWindows() {
    for (auto& [name, window] : windows) {
        if (window->isShown()) {
            window->draw();
        }
    }
}
