#include "EditorLayer.h"

namespace Laura
{

	EditorLayer::EditorLayer(std::shared_ptr<Renderer> renderer, std::shared_ptr<Asset::ResourcePool> resourcePool, std::shared_ptr<Asset::Manager> assetManager, std::shared_ptr<Profiler> profiler)
		: m_Renderer(renderer),
		m_ResourcePool(resourcePool),
		m_AssetManager(assetManager),
		m_Profiler(profiler),

		m_EditorState(std::make_shared<EditorState>()),
		m_ThemeManager(std::make_shared<ThemeManager>()),

		m_InspectorPanel(m_EditorState, m_ThemeManager),
		m_SceneHierarchyPanel(m_EditorState, m_ThemeManager),
		m_ThemesPanel(m_EditorState, m_ThemeManager),
		m_ProfilerPanel(m_EditorState, m_ThemeManager)
	{
		setLayerName("EditorLayer");
	}


	void EditorLayer::onAttach()
	{
		deserializeState(m_EditorState);
		std::string statusMessage;
		if (!m_ThemeManager->LoadTheme(m_EditorState->persistent.ThemeFilePath, statusMessage))
		{
			m_EditorState->persistent.ThemeFilePath = "";
			m_ThemeManager->LoadBuiltInDefualtTheme();
			LR_EDITOR_WARN("Failed to load theme: {0}", statusMessage);
		}
		
		m_AssetManager->SetResourcePool(m_ResourcePool.get());
		m_Scene = std::make_shared<Scene>();

		{
			Entity camera = m_Scene->CreateEntity();
			std::string& tag = camera.GetComponent<TagComponent>().Tag;
			tag = std::string("Camera");
			TransformComponent& cameraTransform = camera.AddComponent<TransformComponent>();
			cameraTransform.SetTranslation({ 0.0f, 40.0f, -200.0f });
			CameraComponent cameraComponent = camera.AddComponent<CameraComponent>();
			cameraComponent.fov = 30.0f;
		}
		{
			Entity dragon = m_Scene->CreateEntity();
			std::string& tag = dragon.GetComponent<TagComponent>().Tag;
			tag = std::string("Dragon");
			MeshComponent& dragonMesh = dragon.AddComponent<MeshComponent>();
			TransformComponent& dragonTransform = dragon.AddComponent<TransformComponent>();
			MaterialComponent& dragonMaterial = dragon.AddComponent<MaterialComponent>();
			// TODO: this should be loaded upon opening the editor - asset manager should keep track of the assets to be loaded (serialize/deserialize them)
			LR_GUID guid = m_AssetManager->LoadMesh(std::string(EDITOR_RESOURCES_PATH "Models/stanford_dragon_pbr.glb"));
			//uint32_t guid = m_AssetManager->LoadMesh(std::string(EDITOR_RESOURCES_PATH "Models/stanford_bunny_pbr.glb"));
			//uint32_t guid = m_AssetManager->LoadMesh(std::string(EDITOR_RESOURCES_PATH "Models/sponza_scene.glb"));
			dragonMesh.guid = guid;
		}
		{
			Entity sponza_e = m_Scene->CreateEntity();
			std::string& tag = sponza_e.GetComponent<TagComponent>().Tag;
			tag = std::string("Bunny");
			MeshComponent& sponzaMesh = sponza_e.AddComponent<MeshComponent>();
			TransformComponent& sponzaTransform = sponza_e.AddComponent<TransformComponent>();
			MaterialComponent& sponzaMaterial = sponza_e.AddComponent<MaterialComponent>();
			// TODO: this should be loaded upon opening the editor - asset manager should keep track of the assets to be loaded (serialize/deserialize them)
			LR_GUID guid = m_AssetManager->LoadMesh(std::string(EDITOR_RESOURCES_PATH "Models/stanford_bunny_pbr.glb"));
			//uint32_t guid = m_AssetManager->LoadMesh(std::string(EDITOR_RESOURCES_PATH "Models/stanford_bunny_pbr.glb"));
			//uint32_t guid = m_AssetManager->LoadMesh(std::string(EDITOR_RESOURCES_PATH "Models/sponza_scene.glb"));
			sponzaMesh.guid = guid;
		}

		//{
		//	Entity sponza_e = m_Scene->CreateEntity();
		//	std::string& tag = sponza_e.GetComponent<TagComponent>().Tag;
		//	tag = std::string("Sponza");
		//	MeshComponent& sponzaMesh = sponza_e.AddComponent<MeshComponent>();
		//	TransformComponent& sponzaTransform = sponza_e.AddComponent<TransformComponent>();
		//	sponzaTransform.SetTranslation({ 0.0f, 40.0f, 1000.0f });
		//	MaterialComponent& sponzaMaterial = sponza_e.AddComponent<MaterialComponent>();
		//	// TODO: this should be loaded upon opening the editor - asset manager should keep track of the assets to be loaded (serialize/deserialize them)
		//	LR_GUID guid = m_AssetManager->LoadMesh(std::string(EDITOR_RESOURCES_PATH "Models/sponza_scene.glb"));
		//	//uint32_t guid = m_AssetManager->LoadMesh(std::string(EDITOR_RESOURCES_PATH "Models/stanford_bunny_pbr.glb"));
		//	//uint32_t guid = m_AssetManager->LoadMesh(std::string(EDITOR_RESOURCES_PATH "Models/sponza_scene.glb"));
		//	sponzaMesh.guid = guid;
		//}

		// Most of these are default arguments (not necessary to specify but showing them for clarity)
		m_Renderer->settings.skyboxGuid = m_AssetManager->LoadTexture(EDITOR_RESOURCES_PATH "Skyboxes/kloofendal_48d_partly_cloudy_puresky_4k.hdr", 4);
		m_Renderer->settings.raysPerPixel = 1;
		m_Renderer->settings.bouncesPerRay = 5;
		m_Renderer->settings.maxAABBIntersections = 500;
		m_Renderer->settings.displayBVH = false;
		m_Renderer->settings.ShouldAccumulate = false;

		// The FRAME_WIDTH and FRAME_HEIGHT define the dimensions of the render frame.
		// These values represent the actual number of pixels that the renderer will process to produce the final image.
		// Note: These dimensions are different from the size or aspect ratio of the ImGui viewport window in the editor.
		// The camera's aspect ratio only stretches the image to fit the viewport window correctly
		m_Renderer->settings.Resolution = glm::uvec2(1200, 800);
		m_Renderer->settings.ComputeShaderPath = LR_RESOURCES_PATH "Shaders/PathTracing.comp";

		m_Renderer->Init();
		m_Scene->OnStart();
	}

	void EditorLayer::onEvent(Event* event)
	{
	}

	void EditorLayer::onUpdate()
	{
		m_Scene->OnUpdate();
	}

	// main rendering function called every frame
	void EditorLayer::onImGuiRender()
	{
		ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport(), ImGuiDockNodeFlags_PassthruCentralNode);
		bool showDemo = true;
		ImGui::ShowDemoWindow(&showDemo);
		if (ImGui::BeginMainMenuBar())
		{
			if (ImGui::BeginMenu("View"))
			{
				if (ImGui::MenuItem("Themes")) { m_EditorState->temp.ThemeSettingsPanelOpen = true; }
				if (ImGui::MenuItem("Profiler")) { m_EditorState->temp.ProfilerPanelOpen = true; }
				ImGui::EndMenu();
			}
			ImGui::EndMainMenuBar();
		}

		m_SceneHierarchyPanel.OnImGuiRender(m_Scene);
		m_InspectorPanel.OnImGuiRender(m_Scene);
		if (m_EditorState->temp.ThemeSettingsPanelOpen) { m_ThemesPanel.OnImGuiRender(); }
		
		
		std::shared_ptr<IImage2D> RenderedFrame = m_Renderer->Render(m_Scene.get(), m_ResourcePool.get());

		m_ViewportPanel.OnImGuiRender(RenderedFrame, m_EditorState);
		m_ProfilerPanel.OnImGuiRender(m_Profiler);
	}

	void EditorLayer::onDetach()
	{
		m_Scene->OnShutdown();
		serializeState(m_EditorState); // save the persistent part of the editor state to the file
	}
}