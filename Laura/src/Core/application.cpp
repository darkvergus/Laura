#include "Core/application.h"

namespace Laura 
{

	void Application::init() {
		Log::Init();
		LOG_ENGINE_INFO("C++ version: {0}", __cplusplus);
		
		_Profiler = std::make_shared<Profiler>(500);

		_Window = IWindow::createWindow();
		_LayerStack = std::make_shared<LayerStack>();
		// make window forward events to the layerStack
		_Window->setEventCallback([this](std::shared_ptr<IEvent> event) { _LayerStack->dispatchEvent(event); });

		_ResourcePool = std::make_shared<AssetPool>();
		_AssetManager = std::make_shared<AssetManager>();
		_AssetManager->SetResourcePool(_ResourcePool.get());

		_RendererAPI = IRendererAPI::Create();

		_SceneLayer = std::make_shared<SceneLayer>(_LayerStack, _AssetManager);
		_RenderLayer = std::make_shared<RenderLayer>(_LayerStack, _Profiler, _ResourcePool);

		_LayerStack->PushLayer(_RenderLayer);
		_LayerStack->PushLayer(_SceneLayer);
	}

	void Application::run() {
		init();
		// mainloop
		while (!_Window->shouldClose()) {
			auto t = _Profiler->globalTimer("GLOBAL");
			{
				auto t = _Profiler->timer("Window::OnUpdate()");
				_Window->onUpdate();
			}
			_RendererAPI->Clear({ 0.98f, 0.24f, 0.97f, 1.0f }); // fill the screen with a color (pink)
			{
				auto t = _Profiler->timer("LayerStack::onUpdate()");
				_LayerStack->onUpdate();
			}
		}
		shutdown();
	}
}