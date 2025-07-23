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
		// make window forward events to the layerStack
		_Window->setEventCallback([this](std::shared_ptr<IEvent> event) { _LayerStack->dispatchEvent(event); });
		_ImGuiContextManager = std::make_shared<ImGuiContext>(_Window);
		_ImGuiContextManager->Init();

		_ResourcePool = std::make_shared<Asset::ResourcePool>();
		_AssetManager = std::make_shared<Asset::Manager>();
		_AssetManager->SetResourcePool(_ResourcePool.get());

		_RendererAPI = IRendererAPI::Create();

		_SceneLayer = std::make_shared<SceneLayer>(_LayerStack);
		_RenderLayer = std::make_shared<RenderLayer>(_LayerStack, _Profiler, _AssetManager, _ResourcePool);

		_LayerStack->PushLayer(_RenderLayer);
		_LayerStack->PushLayer(_SceneLayer);
	}

	void Application::run() {
		init();
		// <ENGINE MAINLOOP> //
		while (!_Window->shouldClose()) {
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