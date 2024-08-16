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
		_Window = IWindow::createWindow(); // window also sets up the rendering context (OpenGL, Vulkan ...)
		_RendererAPI = IRendererAPI::Create();
		_LayerStack = std::make_shared<LayerStack>();
		_ImGuiLayer = std::make_shared<ImGuiLayer>(_Window);
		// make window forward events to the layer stack
		_Window->setEventCallback([this](Event* event) { _LayerStack->onEvent(event); });
		_LayerStack->PushLayer(_ImGuiLayer);
		_Renderer = std::make_shared<Renderer>();
	}

	void Application::run()
	{		
		init();
		while (!_Window->shouldClose())
		{
			_Window->onUpdate();
			_RendererAPI->Clear({0.98f, 0.24f, 0.97f, 1.0f}); // fill the screen with a color (pink)
			_LayerStack->onUpdate();

			_ImGuiLayer->Begin();
			_LayerStack->onImGuiRender();
			_ImGuiLayer->End();
			render();
		}
		shutdown();
	}


	void Application::render()
	{
	}

	void Application::shutdown()
	{
		glfwTerminate();
	}
}