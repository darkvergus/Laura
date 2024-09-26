#pragma once

#include "lrpch.h"

#include "Core/IWindow.h"
#include "Core/LayerStack.h"
#include "Core/ImGuiContext.h"
#include "Core/DeltaTime.h"

#include "Renderer/Renderer.h"
#include "Renderer/IRendererAPI.h"

#include "Assets/AssetManager.h"

#include "Events/Events.h"



namespace Laura {

    class Application
    {
    public:
        Application();
        Application(WindowProps windowProps);
        virtual ~Application();
        void run();
    protected:
        std::shared_ptr<IWindow> _Window;
        std::shared_ptr<LayerStack> _LayerStack;
        std::shared_ptr <ImGuiContext> _ImGuiContextManager;
        std::shared_ptr<IRendererAPI> _RendererAPI;
        std::shared_ptr<Renderer> _Renderer;
        std::shared_ptr<AssetManager> _AssetManager;
        std::shared_ptr<DeltaTime> dt;

    protected:
        virtual void init();
        virtual void shutdown();
    };

    Application* CreateApplication();
}

