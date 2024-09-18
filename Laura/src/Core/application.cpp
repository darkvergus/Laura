#include "Core/application.h"

#include "Core/Log.h"
#include "Renderer/ITexture2D.h"
#include "Assets/MeshLoader.h"

namespace Laura {

	Application::Application()
	{
	}

	Application::~Application()
	{
	}

	void Application::init()
	{
		Log::Init();
		_Window = IWindow::createWindow(); // window also sets up the rendering context (OpenGL, Vulkan [not supported yet] ...)
		_LayerStack = std::make_shared<LayerStack>();
		// make window forward events to the layer stack
		_Window->setEventCallback([this](Event* event) { _LayerStack->onEvent(event); });
		
		_RendererAPI = IRendererAPI::Create();
		_Renderer = std::make_shared<Renderer>();

		_ImGuiContextManager = std::make_shared<ImGuiContextManager>(_Window);
		_ImGuiContextManager->Init();

		_AssetManager = std::make_shared<AssetManager>();
	}

	void Application::run()
	{		
		init();
		while (!_Window->shouldClose())
		{
			_Window->onUpdate();
			_RendererAPI->Clear({0.98f, 0.24f, 0.97f, 1.0f}); // fill the screen with a color (pink)
			_LayerStack->onUpdate();

			_ImGuiContextManager->BeginFrame();
			_LayerStack->onImGuiRender(); // all of the rendering onto the screen happens here
			_ImGuiContextManager->EndFrame();
		}
		shutdown();
	}

	void Application::shutdown()
	{
	}
}