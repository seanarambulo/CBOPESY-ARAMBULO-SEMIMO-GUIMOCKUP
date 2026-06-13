#include "GUIApplication.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <iostream>

#ifdef _WIN32
#pragma comment(lib, "opengl32.lib")
#endif

#include "UIConfig.h"
#include "UIManager.h"
#include "Desktop.h"
#include "Taskbar.h"
#include "TaskManagerUI.h"
#include "ProcessWindow.h"
#include "SearchWindow.h"
#include "BIOSBootWindow.h"
#include "StartMenu.h"

// Forward declare GLFW error callback
static void glfw_error_callback(int error, const char* description) {
    std::cerr << "GLFW Error " << error << ": " << description << std::endl;
}

// Window close callback to prevent force exit
static void window_close_callback(GLFWwindow* window) {
    glfwSetWindowShouldClose(window, GLFW_FALSE);
}

GUIApplication::GUIApplication() : window(nullptr) {
}

GUIApplication::~GUIApplication() {
    shutdown();
}

bool GUIApplication::initialize() {
    // Phase 1: Bootstrapping — hardware/window system setup
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
    const char* glsl_version = "#version 330";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
#endif

    window = glfwCreateWindow(1280, 720, "CSOPESY", nullptr, nullptr);
    if (window == nullptr)
        return false;
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync

    // Intercept the close button
    glfwSetWindowCloseCallback(window, window_close_callback);

    // Initialize Dear ImGui (equivalent to kernel entry point)
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
    
    // Enable DPI awareness (Custom implementation using UIConfig)

    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    // Phase 2: Kernel Initialization — set up data structures
    UIConfig::initialize();
    UIManager::getInstance().initialize();

    float scaleFactor = UIConfig::getScaleFactor();
    ImGui::GetStyle().ScaleAllSizes(scaleFactor);
    io.FontGlobalScale = scaleFactor;

    // Phase 3: Start System Services — launch core windows
    auto desktop = std::make_shared<Desktop>();
    auto taskbar = std::make_shared<Taskbar>();
    auto taskMgr = std::make_shared<TaskManagerUI>();
    auto processWindow = std::make_shared<ProcessWindow>();
    auto searchWindow = std::make_shared<SearchWindow>();
    auto biosWindow = std::make_shared<BIOSBootWindow>();
    auto startMenu = std::make_shared<StartMenu>();
    
    UIManager::getInstance().registerWindow("desktop", desktop);
    UIManager::getInstance().registerWindow("taskbar", taskbar);
    UIManager::getInstance().registerWindow("taskManager", taskMgr);
    UIManager::getInstance().registerWindow("process", processWindow);
    UIManager::getInstance().registerWindow("search", searchWindow);
    UIManager::getInstance().registerWindow("bios", biosWindow);
    UIManager::getInstance().registerWindow("startMenu", startMenu);

    // Initial state: hide desktop and taskbar, show BIOS
    desktop->hide();
    taskbar->hide();
    startMenu->hide();
    biosWindow->show();

    return true;
}

void GUIApplication::run() {
    ImGuiIO& io = ImGui::GetIO();
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    // Phase 4: Enter Main Loop — handle events, dispatch, I/O
    while (!glfwWindowShouldClose(window) && !UIManager::getInstance().isApplicationClosing()) {
        // Handle interrupts / poll for input events
        glfwPollEvents();

        // Dispatch "processes" — render all active windows
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // Update application logic
        updateLogic();

        // Render all UI components
        renderFrame();

        // Flush output — present the rendered frame
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
    UIManager::getInstance().updateAllWindows();
}

void GUIApplication::renderFrame() {
    UIManager::getInstance().renderAllWindows();
}

void GUIApplication::shutdown() {
    // Phase 5: Shutdown and Cleanup
    if (window != nullptr) {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();

        glfwDestroyWindow(window);
        glfwTerminate();
        window = nullptr;
    }
}
