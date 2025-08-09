#pragma once

#include "lrpch.h"
#include "Core/IWindow.h"
#include "Core/Layers/LayerStack.h"
#include "Core/Layers/RenderLayer.h"
#include "Core/Profiler.h"
#include "Project/ProjectManager.h"
#include "Renderer/IRendererAPI.h"
#include "Events/IEvent.h"

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

        std::shared_ptr<ProjectManager> _ProjectManager;

        std::shared_ptr<RenderLayer>    _RenderLayer;

    protected:
        virtual void init();
        virtual void shutdown();
    };

    Application* CreateApplication();
}

