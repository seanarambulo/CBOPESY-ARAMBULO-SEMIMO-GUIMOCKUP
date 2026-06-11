#include "GUIApplication.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <iostream>

#include "UIConfig.h"
#include "UIManager.h"
#include "TaskManagerUI.h"
#include "BIOSBootWindow.h"
#include "ProcessWindow.h"

// Forward declare GLFW error callback
static void glfw_error_callback(int error, const char* description) {
    std::cerr << "GLFW Error " << error << ": " << description << std::endl;
}

GUIApplication::GUIApplication() : window(nullptr) {
}

GUIApplication::~GUIApplication() {
    shutdown();
}

bool GUIApplication::initialize() {
    // 1. OS Bootstrapping: Initialize GLFW
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit())
        return false;

    // Decide GL+GLSL versions
#if defined(__APPLE__)
    const char* glsl_version = "#version 150";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#else
    const char* glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
#endif

    // Create window with graphics context
    window = glfwCreateWindow(1280, 720, "OS Mockup", nullptr, nullptr);
    if (window == nullptr)
        return false;
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    setupImGui();

    // 2. Kernel Initialization Phase: Register services
    UIManager* uiManager = UIManager::getInstance();

    desktop = std::make_shared<Desktop>();
    taskbar = std::make_shared<Taskbar>();
    
    uiManager->registerWindow(desktop);
    uiManager->registerWindow(taskbar);
    uiManager->registerWindow(std::make_shared<TaskManagerUI>());
    uiManager->registerWindow(std::make_shared<BIOSBootWindow>());
    uiManager->registerWindow(std::make_shared<ProcessWindow>());

    return true;
}

void GUIApplication::setupImGui() {
    // Enable DPI awareness and load fonts
    ImGuiIO& io = ImGui::GetIO();
    
    // In a real application, you might get the monitor width here
    int windowWidth, windowHeight;
    glfwGetWindowSize(window, &windowWidth, &windowHeight);
    
    UIConfig::getInstance()->initialize((float)windowWidth);
    float scaleFactor = UIConfig::getInstance()->getScaleFactor();

    // Set global scale
    ImGui::GetStyle().ScaleAllSizes(scaleFactor);
    io.FontGlobalScale = scaleFactor;

    // Here you would load fonts, e.g.:
    // io.Fonts->AddFontFromFileTTF("path/to/font.ttf", 16.0f);
}

void GUIApplication::run() {
    ImGuiIO& io = ImGui::GetIO();
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    // The OS infinite main loop
    while (!glfwWindowShouldClose(window)) {
        // 1. Poll Events
        glfwPollEvents();

        // 2. Start the frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // 3. Process Logic
        updateLogic();

        // 4. Render UI
        renderFrame();

        // 5. Swap Buffers
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }
}

void GUIApplication::updateLogic() {
    // Update background process states
    // Handle DPI scaling changes on resize
    int windowWidth, windowHeight;
    glfwGetWindowSize(window, &windowWidth, &windowHeight);
    UIConfig::getInstance()->initialize((float)windowWidth);
}

void GUIApplication::renderFrame() {
    // Explicitly draw background services first
    desktop->draw();
    taskbar->draw();
    
    // Draw all user-opened windows floating above
    UIManager::getInstance()->renderAllWindows();
}

void GUIApplication::shutdown() {
    // Graceful termination
    if (window != nullptr) {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();

        glfwDestroyWindow(window);
        glfwTerminate();
        window = nullptr;
    }
}
