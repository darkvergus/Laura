#include "EditorLayer.h"

namespace Laura
{

	EditorLayer::EditorLayer(std::shared_ptr<Renderer> renderer, std::shared_ptr<AssetManager> assetManager)
		: m_Renderer(renderer), 
		m_AssetManager(assetManager),
		m_EditorState(std::make_shared<EditorState>()),
		m_ThemeManager(std::make_shared<ThemeManager>()),
		m_InspectorPanel(m_EditorState, m_ThemeManager)
	{
		setLayerName("EditorLayer");
	}

	void EditorLayer::onAttach()
	{
		m_Scene = std::make_shared<Scene>(); // Call the default constructor of the Scene class
		// Setting up the Skybox
		
		// TODO make the api for the texture channels more user friendly and less error prone
		// currently there is no way to catch these errors and they lead to hard to debug crashes
		uint32_t skyboxTextureID = m_AssetManager->LoadTexture(EDITOR_RESOURCES_PATH "Skyboxes/kloofendal_48d_partly_cloudy_puresky_4k.hdr", 4);
		m_Scene->skybox = std::make_shared<Skybox>(skyboxTextureID);
		

		// Adding a CAMERA to the scene
		Entity camera = m_Scene->CreateEntity();
		{
			std::string& tag = camera.GetComponent<TagComponent>().Tag;
			tag = std::string("Camera");
		}
		TransformComponent& cameraTransform = camera.AddComponent<TransformComponent>();
		cameraTransform.SetTranslation({ 0.0f, 40.0f, -200.0f });
		CameraComponent cameraComponent = camera.AddComponent<CameraComponent>();
		// Using the default values for the camera component
		cameraComponent.fov = 30.0f;
		//cameraComponent.aspectRatio = 16.0f/9.0f;


		// Adding a 3D MODEL to the scene
		Entity dragon = m_Scene->CreateEntity();
		{
			
			std::string& tag = dragon.GetComponent<TagComponent>().Tag;
			tag = std::string("Dragon");

			MeshComponent& dragonMesh = dragon.AddComponent<MeshComponent>();
			TransformComponent& dragonTransform = dragon.AddComponent<TransformComponent>();
			MaterialComponent& dragonMaterial = dragon.AddComponent<MaterialComponent>();

			uint32_t ID = m_AssetManager->LoadMesh(std::string(EDITOR_RESOURCES_PATH "Models/stanford_dragon_pbr.glb"));
			//uint32_t ID = m_AssetManager->LoadMesh(std::string(EDITOR_RESOURCES_PATH "Models/stanford_bunny_pbr.glb"));
			//uint32_t ID = m_AssetManager->LoadMesh(std::string(EDITOR_RESOURCES_PATH "Models/sponza_scene.glb"));
			dragonMesh.SetID(ID);
		}
		m_Renderer->renderSettings.raysPerPixel = 1;
		m_Renderer->renderSettings.bouncesPerRay = 5;
		m_Renderer->renderSettings.maxAABBIntersections = 10000;
		m_Renderer->renderSettings.displayBVH = false;

		// The FRAME_WIDTH and FRAME_HEIGHT define the dimensions of the render frame.
		// These values represent the actual number of pixels that the renderer will process to produce the final image.
		// Note: These dimensions are different from the size or aspect ratio of the ImGui viewport window in the editor.
		// The camera's aspect ratio only stretches the image to fit the viewport window correctly
		#define FRAME_WIDTH 300.0f
		#define FRAME_HEIGHT 150.0f
		m_Renderer->SetFrameResolution(glm::vec2(FRAME_WIDTH, FRAME_HEIGHT));

		/// ------------- SCRIPTING -------------- ///
		// Test SCRIPT (will be in its own file in the future)
		class TestScript : public Script
		{
			virtual void OnCreate() override 
			{
				LR_APP_INFO("TestScript::OnCreate (called in scene.OnStart())");
			}

			virtual void OnUpdate() override
			{
				LR_APP_INFO("TestScript::OnUpdate (this should get called every frame)");
			}

			virtual void OnDestroy() override
			{
				LR_APP_INFO("TestScript::OnDestroy (called once the entity gets destroyed)");
			}
		};
		
		// Testing the TEST SCRIPT on a TEST ENTITY
		Entity testEntity = m_Scene->CreateEntity();
		{
			std::string& tag = testEntity.GetComponent<TagComponent>().Tag;
			tag = std::string("TestScript");
		}
		testEntity.AddComponent<ScriptComponent>(new TestScript());
		//m_Scene.DestroyEntity(testEntity); // testing the script's OnDestroy() function
		/// ---------------------------------------- ///

		m_Scene->OnStart();

		// setting up the renderer
		std::shared_ptr<LoadedTexture> texture = m_AssetManager->GetTexture(skyboxTextureID);
		m_Renderer->BeginScene(camera, texture);
		MeshComponent dragon_mesh = dragon.GetComponent<MeshComponent>();
		TransformComponent dragon_transform = dragon.GetComponent<TransformComponent>();
		MaterialComponent dragon_material = dragon.GetComponent<MaterialComponent>();
		m_Renderer->Submit(m_AssetManager->GetMesh(dragon_mesh.GetID()), dragon_transform, dragon_material);
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
		
		if (ImGui::BeginMainMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				ImGui::MenuItem("New", "Ctrl+N");
				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Edit"))
			{
				if (ImGui::MenuItem("Undo", "CTRL+Z")) {}
				if (ImGui::MenuItem("Redo", "CTRL+Y", false, false)) {}  // Disabled item
				ImGui::Separator();
				if (ImGui::MenuItem("Cut", "CTRL+X")) {}
				if (ImGui::MenuItem("Copy", "CTRL+C")) {}
				if (ImGui::MenuItem("Paste", "CTRL+V")) {}
				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Settings"))
			{
				if (ImGui::MenuItem("Themes")) { m_EditorState->ThemeSettingsPanelOpen = true; }
				ImGui::EndMenu();
			}

			ImGui::EndMainMenuBar();
		}

		bool showDemoWindow = false;
		ImGui::ShowDemoWindow(&showDemoWindow);
		m_SceneHierarchyPanel.OnImGuiRender(m_Scene, m_EditorState);
		m_InspectorPanel.OnImGuiRender(m_Scene);
		if (m_EditorState->ThemeSettingsPanelOpen) { m_ThemesPanel.OnImGuiRender(m_EditorState, m_ThemeManager); }

		//m_Renderer->SetFrameResolution(glm::vec2(viewportSize.x, viewportSize.y));
		//m_Renderer->renderSettings.accumulateFrames = false;
		//m_Renderer->UpdateRenderSettingsUBO();

		std::shared_ptr<IImage2D> RenderedFrame = m_Renderer->RenderScene();
		m_ViewportPanel.OnImGuiRender(RenderedFrame, m_EditorState);
	}

	void EditorLayer::onDetach()
	{
		m_Scene->OnShutdown();
	}

}