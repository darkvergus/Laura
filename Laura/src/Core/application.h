#pragma once

#include "lrpch.h"
// Forward declarations to reduce compilation dependencies
namespace Laura {
    class IWindow;
    class LayerStack;
    class RenderLayer;
    class Profiler;
    class ProjectManager;
    class IRendererAPI;
}

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

    Application* CreateApplication(const std::filesystem::path& exeDir);
}

