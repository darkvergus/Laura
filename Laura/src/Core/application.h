#pragma once

#include "lrpch.h"

#include "Core/IWindow.h"
#include "Core/Layers/LayerStack.h"
#include "Core/ImGuiContext.h"

#include "Renderer/Renderer.h"
#include "Renderer/IRendererAPI.h"

#include "Assets/Assets.h"

#include "Events/Events.h"

#include "Core/Profiler.h"


namespace Laura {

    class Application
    {
    public:
        Application() = default;
        //Application(WindowProps windowProps);
        virtual ~Application() = default;
        void run();
    protected:
        std::shared_ptr<IWindow>        _Window;
        std::shared_ptr<LayerStack>     _LayerStack;
        std::shared_ptr<ImGuiContext>   _ImGuiContextManager;
        std::shared_ptr<IRendererAPI>   _RendererAPI;
        std::shared_ptr<Renderer>       _Renderer;
        std::shared_ptr<Profiler>       _Profiler;

        std::shared_ptr<Asset::ResourcePool> _ResourcePool;
        std::shared_ptr<Asset::Manager>      _AssetManager;

    protected:
        virtual void init();
        inline virtual void shutdown() {};
    };

    Application* CreateApplication();
}

