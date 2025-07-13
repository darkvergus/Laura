#include "Core/application.h"

namespace Laura 
{
	void Application::init()
	{
		Log::Init();
		LOG_ENGINE_INFO("C++ version: {0}", __cplusplus);
		
		_Profiler = std::make_shared<Profiler>(500);

		_Window = IWindow::createWindow();
		_LayerStack = std::make_shared<LayerStack>();
		// make window forward events to the layer stack
		_Window->setEventCallback([this](Event* event) { _LayerStack->onEvent(event); });
		
		_ImGuiContextManager = std::make_shared<ImGuiContext>(_Window);
		_ImGuiContextManager->Init();

		_ResourcePool = std::make_shared<Asset::ResourcePool>();
		_AssetManager = std::make_shared<Asset::Manager>();

		_RendererAPI = IRendererAPI::Create();

		_Renderer = std::make_shared<Renderer>(_Profiler);
	}

	void Application::run()
	{		
		init();
		while (!_Window->shouldClose())
		{
			auto t = _Profiler->globalTimer("GLOBAL");
			{
				auto t = _Profiler->timer("Window::OnUpdate()");
				_Window->onUpdate();
			}

			_RendererAPI->Clear({ 0.98f, 0.24f, 0.97f, 1.0f }); // fill the screen with a color (pink)
			
			_LayerStack->onUpdate();

			{
				auto t = _Profiler->timer("LayerStack::onImGuiRender()");
				_ImGuiContextManager->BeginFrame();
				_LayerStack->onImGuiRender(); // all of the rendering onto the screen happens here
				_ImGuiContextManager->EndFrame();
			}
		}
		shutdown();
	}
}