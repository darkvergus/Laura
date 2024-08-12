#pragma once

#include "lrpch.h"

#include <imgui.h>

// THESE NEED TO GO ASAP // 
#include <GL/glew.h>
#include "Platform/OpenGL/OpenGLdebugFuncs.h"
///////////////////////////

#include "Core/IWindow.h"
#include "Core/LayerStack.h"
#include "Core/ImGuiLayer.h"
#include "Events/Events.h"
#include "Platform/windows/GLFWWindow.h"

#include "Renderer/Renderer.h"

#include "Components/Camera.h"
#include "Entity/Environment.h"

namespace Laura {

    class Application
    {
    protected:
        // new stuff
        IWindow* m_Window;
        LayerStack* m_LayerStack;
        ImGuiLayer* m_ImGuiLayer;
        std::shared_ptr<Renderer> m_Renderer;

        // should move to sandbox
        Camera m_Camera;
        EnvironmentEntity m_Environment;

        // temporary
        glm::vec2 prevViewportWindowSize, prevViewportWindowPos, viewportSize;
        ImVec2 topLeftTextureCoords, bottomLeftTextureCoords;
        uint32_t prev_viewport_mouseX, prev_viewport_mouseY;


    public:
        Application();
        virtual ~Application();
        void run();

    protected:
        virtual void init();
        virtual void render();
        virtual void shutdown();
    };

    Application* CreateApplication();
}

