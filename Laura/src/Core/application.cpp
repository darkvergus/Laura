#include "core/application.h"

#include "Core/Log.h"
#include "renderer/ITexture2D.h"
#include "Assets/MeshLoader.h"

namespace Laura {

	Application::Application() :
		prevViewportWindowSize		(glm::vec2(0.0f, 0.0f)),
		prevViewportWindowPos		(glm::vec2(0.0f, 0.0f)),
		prev_viewport_mouseX		(0),
		prev_viewport_mouseY		(0)
	{}

	Application::~Application(){}

	void Application::run()
	{		
		init();
		float aspectRatio = 16.0f / 9.0f;
		while (!m_Window->shouldClose())
		{
				
			m_Window->onUpdate();
			m_LayerStack->onUpdate();

			m_ImGuiLayer->Begin();
			



			GLCall(glClear(GL_COLOR_BUFFER_BIT));
			GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
		
			bool showDemoWindow = true;
			ImGui::ShowDemoWindow(&showDemoWindow);
			
			m_Renderer->renderSettings.accumulateFrames = true;

			/// VIEWPORT WINDOW
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
			ImGui::Begin("Viewport", nullptr, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse);
		
			glm::vec2 viewportWindowSize = glm::vec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y);
			if (viewportWindowSize != prevViewportWindowSize)
			{
				prevViewportWindowSize = viewportWindowSize;
		
				double parentAspect = double(viewportWindowSize.x) / double(viewportWindowSize.y);
				// check if the parent aspect is smaller or equal to camera aspect (width is the limiting factor so the window will span the entire width)
				if (parentAspect <= aspectRatio)
				{
					viewportSize.x = viewportWindowSize.x; // width stays the same
					viewportSize.y = ceil(viewportWindowSize.x / aspectRatio); // calculate the height based on the aspect ratio
				}
				// (height is the limiting factor so the window will span the entire height)
				else if (parentAspect > aspectRatio)
				{
					viewportSize.y = viewportWindowSize.y; // height stays the same
					viewportSize.x = ceil(viewportWindowSize.y * aspectRatio); // calculate the width based on the aspect ratio
				}

				m_Renderer->renderSettings.viewportDimensions = glm::vec2(viewportSize.x, viewportSize.y);
				m_Renderer->renderSettings.accumulateFrames = false;
				m_Renderer->UpdateRenderSettings();
		
				topLeftTextureCoords.x = (viewportWindowSize.x - viewportSize.x) / 2.0f;
				topLeftTextureCoords.y = (viewportWindowSize.y - viewportSize.y) / 2.0f;
				// viewport offset
				topLeftTextureCoords.x += ImGui::GetWindowPos().x;
				topLeftTextureCoords.y += ImGui::GetWindowPos().y;
		
				bottomLeftTextureCoords.x = topLeftTextureCoords.x + viewportSize.x;
				bottomLeftTextureCoords.y = topLeftTextureCoords.y + viewportSize.y;
			}
		
		
			// check if the viewport window position changed
			glm::vec2 viewportWindowPos = glm::vec2(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y);
			if (viewportWindowPos != prevViewportWindowPos)
			{
				prevViewportWindowPos = viewportWindowPos;
		
				topLeftTextureCoords.x = (viewportWindowSize.x - viewportSize.x) / 2.0f;
				topLeftTextureCoords.y = (viewportWindowSize.y - viewportSize.y) / 2.0f;
				// viewport offset
				topLeftTextureCoords.x += viewportWindowPos.x;
				topLeftTextureCoords.y += viewportWindowPos.y;
		
				bottomLeftTextureCoords.x = topLeftTextureCoords.x + viewportSize.x;
				bottomLeftTextureCoords.y = topLeftTextureCoords.y + viewportSize.y;
			}
		
			std::shared_ptr<IImage2D> RenderedFrame = m_Renderer->RenderScene();
		
			ImDrawList* drawList = ImGui::GetWindowDrawList();
			drawList->AddImage((ImTextureID)RenderedFrame->GetID(), topLeftTextureCoords, bottomLeftTextureCoords, { 0, 1 }, { 1, 0 });
		
			ImGui::PopStyleVar();
			ImGui::End();

			m_ImGuiLayer->End();

			render();
		}
		shutdown();
	}


	void Application::init()
	{
		Log::Init();
		m_Window = IWindow::createWindow();
		m_LayerStack = new LayerStack();
		m_ImGuiLayer = new ImGuiLayer(m_Window);
		// make window forward events to the layer stack
		m_Window->setEventCallback([this](Event* event) { m_LayerStack->onEvent(event); });
		m_LayerStack->PushLayer(m_ImGuiLayer);
		m_Renderer = std::make_shared<Renderer>();
	}

	void Application::render()
	{
	}

	void Application::shutdown()
	{
		glfwTerminate();
		delete m_LayerStack;
		delete m_Window;
	}
}