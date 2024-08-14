#include "Core/application.h"

#include "Core/Log.h"
#include "Renderer/ITexture2D.h"
#include "Assets/MeshLoader.h"

namespace Laura {

	Application::Application()
	{
	}

	Application::~Application(){}

	void Application::init()
	{
		Log::Init();
		_Window = IWindow::createWindow();
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

		//float aspectRatio = 16.0f / 9.0f;
		while (!_Window->shouldClose())
		{
			_Window->onUpdate();
			GLCall(glClear(GL_COLOR_BUFFER_BIT));
			GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
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