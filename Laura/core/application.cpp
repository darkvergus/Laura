#include "core/application.h"



#include "Core/Log.h"
#include "renderer/ITexture.h"
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
				
			Application::update();
			m_ImGuiLayer->Begin();
			
			GLCall(glClear(GL_COLOR_BUFFER_BIT));
			GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
		
			bool showDemoWindow = true;
			ImGui::ShowDemoWindow(&showDemoWindow);
			
			renderer->renderSettings.accumulateFrames = true;

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

				renderer->renderSettings.viewportDimensions = glm::vec2(viewportSize.x, viewportSize.y);
				renderer->renderSettings.accumulateFrames = false;
				renderer->UpdateRenderSettings();
		
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
		
			std::shared_ptr<ITexture> RenderedFrame = renderer->RenderScene();
		
			ImDrawList* drawList = ImGui::GetWindowDrawList();
			drawList->AddImage((ImTextureID)RenderedFrame->GetID(), topLeftTextureCoords, bottomLeftTextureCoords, { 0, 1 }, { 1, 0 });
		
			ImGui::PopStyleVar();
			ImGui::End();

			m_ImGuiLayer->End();

			render(); // Does nothing for some reason?
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
		m_Window->setEventCallback([this](Event* event) { m_LayerStack->dispatchEvent(event); });
		m_LayerStack->PushLayer(m_ImGuiLayer);

		renderer = new Renderer();
		renderer->SetAPI(IRendererAPI::API::OpenGL);
		
		m_Camera.transform.setPosition({ 0.0f, 40.0f, -200.0f });
		m_Camera.transform.setRotation({ 0.0f, 0.0f, 0.0f });
		m_Camera.setFOV(90.0f);

		m_Environment.skybox.changeType(SkyboxType::SKYBOX_TEXTURE);
		m_Environment.skybox.setTexturePath(std::string(CORE_RESOURCES_PATH "Skyboxes/Metro_default.hdr"));
		
		renderer->renderSettings.viewportDimensions = glm::vec2(1280, 720);
		renderer->renderSettings.raysPerPixel = 1;
		renderer->renderSettings.bouncesPerRay = 5;
		renderer->renderSettings.maxAABBIntersections = 10000;
		renderer->renderSettings.displayBVH = false;

		renderer->BeginScene(m_Camera, m_Environment);

		MeshComponent model3D = loadMesh(std::string(APP_RESOURCES_PATH "models/stanford_dragon_pbr.glb"));
		//MeshComponent model3D = loadMesh(std::string(APP_RESOURCES_PATH "models/suzanne_high_poly_rotated.glb")),
		//MeshComponent model3D = loadMesh(std::string(APP_RESOURCES_PATH "models/tiny_house.glb")),
		//MeshComponent model3D = loadMesh(std::string(APP_RESOURCES_PATH "models/sponza_scene.glb")),
		//MeshComponent model3D = loadMesh(std::string(APP_RESOURCES_PATH "models/stanford_bunny.obj")),
		renderer->SubmitMesh(model3D);
	}

	void Application::update()
	{
		m_Window->onUpdate();
		m_LayerStack->onUpdate();
	}

	void Application::render()
	{
	}

	void Application::shutdown()
	{
		glfwTerminate();
		delete renderer;
		delete m_LayerStack;
		delete m_Window;
	}

	Application* createApplication()
	{
		return new Application();
	}
} // namespace Laura