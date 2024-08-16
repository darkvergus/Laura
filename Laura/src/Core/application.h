#pragma once

#include "lrpch.h"

#include "Core/IWindow.h"
#include "Core/LayerStack.h"
#include "Core/ImGuiLayer.h"
#include "Core/DeltaTime.h"

#include "Renderer/Renderer.h"
#include "Renderer/IRendererAPI.h"

#include "Events/Events.h"

#include "Components/Camera.h"
#include "Entity/Environment.h"



namespace Laura {

    class Application
    {
    public:
        Application();
        virtual ~Application();
        void run();
    protected:
        std::shared_ptr<IWindow> _Window;
        std::shared_ptr<LayerStack> _LayerStack;
        std::shared_ptr <ImGuiLayer> _ImGuiLayer;
        std::shared_ptr<IRendererAPI> _RendererAPI;
        std::shared_ptr<Renderer> _Renderer;
        std::shared_ptr<DeltaTime> dt;

    protected:
        virtual void init();
        virtual void render();
        virtual void shutdown();
    };

    Application* CreateApplication();
}

