#include "Core/application.h"

#include "Core/Log.h"
#include "Renderer/ITexture2D.h"
#include "Assets/MeshLoader.h"

namespace Laura 
{
	void Application::init()
	{
		Log::Init();
		_Window = IWindow::createWindow(); // window also sets up the rendering context (OpenGL, Vulkan [not supported yet] ...)
		_LayerStack = std::make_shared<LayerStack>();
		// make window forward events to the layer stack
		_Window->setEventCallback([this](Event* event) { _LayerStack->onEvent(event); });
		
		_ImGuiContextManager = std::make_shared<ImGuiContext>(_Window);
		_ImGuiContextManager->Init();

		_AssetManager = std::make_shared<AssetManager>();
		_SceneManager = std::make_shared<SceneManager>(_AssetManager);

		_RendererAPI = IRendererAPI::Create();
		_Renderer = std::make_shared<Renderer>();

		_Profiler = std::make_shared<Profiler>(500);
	}

	void Application::run()
	{		
		init();
		while (!_Window->shouldClose())
		{
			auto t = _Profiler->globalTimer("GLOBAL");

			{
				auto t = _Profiler->timer("Window.OnUpdate()");
				_Window->onUpdate();
			}

			{
				auto t = _Profiler->timer("RendererAPI.Clear()");
				_RendererAPI->Clear({ 0.98f, 0.24f, 0.97f, 1.0f }); // fill the screen with a color (pink)
			}

			_LayerStack->onUpdate();
			
			{
				auto t = _Profiler->timer("Rendering");
				_ImGuiContextManager->BeginFrame();
				_LayerStack->onImGuiRender(); // all of the rendering onto the screen happens here
				_ImGuiContextManager->EndFrame();
			}
		}
		shutdown();
	}

	void Application::shutdown()
	{
	}
}