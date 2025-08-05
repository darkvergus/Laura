#include "lrpch.h"
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <IconsFontAwesome6.h>
#include <iconsFontAwesome6Brands.h>
#include <GLFW\glfw3.h>
#include <implot.h>
#include "ImGuiContext.h"

namespace Laura 
{

    ImGuiContext::ImGuiContext(std::shared_ptr<IWindow> window)
        : m_Window(window), m_FontRegistry(std::make_shared<ImGuiContextFontRegistry>()) {
    }

    ImGuiContext::~ImGuiContext() {
        ImPlot::DestroyContext();
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }

    void ImGuiContext::Init() {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImPlot::CreateContext();
 
        ImGuiIO& io = ImGui::GetIO();
		ImFontConfig config;
		config.OversampleH = 3;
		config.OversampleV = 3;
		config.PixelSnapH = true;
        io.FontDefault = io.Fonts->AddFontFromFileTTF(EDITOR_RESOURCES_PATH "Fonts/Inter/Inter_18pt-Medium.ttf", 15.0f, &config);
        //io.FontDefault = io.Fonts->AddFontFromFileTTF(EDITOR_RESOURCES_PATH "Fonts/source-sans/SourceSans3-Regular.ttf", 15.0f, &config);

        ImFontConfig iconConfig;
        iconConfig.MergeMode = true;  // Merge Font Awesome with the default font
        static const ImWchar iconRanges[] = { ICON_MIN_FA, ICON_MAX_FA, 0 };
        io.Fonts->AddFontFromFileTTF(EDITOR_RESOURCES_PATH "Fonts/fontawesome-free-6.6.0-desktop/Font Awesome 6 Free-Solid-900.otf", 13.0f, &iconConfig, iconRanges);

        ImFontConfig highResIconConfig;
		highResIconConfig.MergeMode = false; // dont merge to main font (separate)
		highResIconConfig.PixelSnapH = true;
		m_FontRegistry->HighResIcons = io.Fonts->AddFontFromFileTTF(
			EDITOR_RESOURCES_PATH "Fonts/fontawesome-free-6.6.0-desktop/Font Awesome 6 Free-Solid-900.otf",
			40.0f,
			&highResIconConfig,
			iconRanges
		);
        ImGui::GetIO().UserData = m_FontRegistry.get();

        (void)io;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
        io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows
        //io.ConfigViewportsNoAutoMerge = true;
        //io.ConfigViewportsNoTaskBarIcon = true;


        // Setup Dear ImGui style
        ImGui::StyleColorsDark();

        // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
        ImGuiStyle& style = ImGui::GetStyle();
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
            style.WindowRounding = 0.0f;
            style.TabRounding = 0.0f;
            style.Colors[ImGuiCol_WindowBg].w = 1.0f;
        }

        style.WindowTitleAlign = ImVec2(0.5f, 0.5f);
        style.WindowMenuButtonPosition = ImGuiDir_None; // remove the menu button from the titlebar

        ImGui_ImplGlfw_InitForOpenGL(static_cast<GLFWwindow*>(m_Window->getNativeWindow()), true); // true: install callbacks in the GLFW window
        ImGui_ImplOpenGL3_Init("#version 460");
    }

    void ImGuiContext::BeginFrame() {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
    }

    void ImGuiContext::EndFrame() {
        ImGuiIO& io = ImGui::GetIO();
        io.DisplaySize = ImVec2((float)m_Window->getWidth(), (float)m_Window->getHeight());

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
            GLFWwindow* backup_current_context = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backup_current_context);
        }
    }
}
