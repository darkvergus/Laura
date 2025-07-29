#pragma once

#include "lrpch.h"
#include "Core/IWindow.h"
#include "Core/Layers/LayerStack.h"
#include "Core/Layers/SceneLayer.h"
#include "Core/Layers/RenderLayer.h"
#include "Renderer/Renderer.h"
#include "Renderer/IRendererAPI.h"
#include "Project/Assets/AssetManager.h"
#include "Events/IEvent.h"
#include "Core/Profiler.h"

namespace Laura 
{

    class Application {
    public:
        Application() = default;
        //Application(WindowProps windowProps);
        virtual ~Application() = default;
        void run();
        
    protected:
        std::shared_ptr<IWindow>        _Window;

        std::shared_ptr<LayerStack>     _LayerStack;
        std::shared_ptr<IRendererAPI>   _RendererAPI;
        std::shared_ptr<Profiler>       _Profiler;

        std::shared_ptr<ResourcePool> _ResourcePool;
        std::shared_ptr<AssetManager>      _AssetManager;

        std::shared_ptr<SceneLayer>     _SceneLayer;
        std::shared_ptr<RenderLayer>    _RenderLayer;

    protected:
        virtual void init();
        inline virtual void shutdown() {};
    };

    Application* CreateApplication();
}

