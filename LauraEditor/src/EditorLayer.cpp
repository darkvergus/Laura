#include "EditorLayer.h"

namespace Laura
{

	EditorLayer::EditorLayer(std::shared_ptr<Renderer> renderer, std::shared_ptr<SceneManager> sceneManager, std::shared_ptr<AssetManager> assetManager)
		: m_Renderer(renderer),
		m_SceneManager(sceneManager),
		m_AssetManager(assetManager),

		m_EditorState(std::make_shared<EditorState>()),
		m_ThemeManager(std::make_shared<ThemeManager>()),
		m_InspectorPanel(m_EditorState, m_ThemeManager),
		m_SceneHierarchyPanel(m_EditorState, m_ThemeManager),
		m_ThemesPanel(m_EditorState, m_ThemeManager)
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
		
		// TODO: Scene serialization/deserialization
		m_Scene = std::make_shared<Scene>();

		// TODO make the api for the texture channels more user friendly and less error prone
		// currently there is no way to catch these errors and they lead to hard to debug crashes
		{
			GUID guid = m_AssetManager->LoadTexture(EDITOR_RESOURCES_PATH "Skyboxes/kloofendal_48d_partly_cloudy_puresky_4k.hdr", 4);
			m_Scene->skyboxGuid = guid;
		}
		
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
			GUID guid = m_AssetManager->LoadMesh(std::string(EDITOR_RESOURCES_PATH "Models/stanford_dragon_pbr.glb"));
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
			GUID guid = m_AssetManager->LoadMesh(std::string(EDITOR_RESOURCES_PATH "Models/stanford_bunny_pbr.glb"));
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
		//	MaterialComponent& sponzaMaterial = sponza_e.AddComponent<MaterialComponent>();
		//	// TODO: this should be loaded upon opening the editor - asset manager should keep track of the assets to be loaded (serialize/deserialize them)
		//	GUID guid = m_AssetManager->LoadMesh(std::string(EDITOR_RESOURCES_PATH "Models/sponza_scene.glb"));
		//	//uint32_t guid = m_AssetManager->LoadMesh(std::string(EDITOR_RESOURCES_PATH "Models/stanford_bunny_pbr.glb"));
		//	//uint32_t guid = m_AssetManager->LoadMesh(std::string(EDITOR_RESOURCES_PATH "Models/sponza_scene.glb"));
		//	sponzaMesh.guid = guid;
		//}

		m_Renderer->renderSettings.raysPerPixel = 1;
		m_Renderer->renderSettings.bouncesPerRay = 5;
		m_Renderer->renderSettings.maxAABBIntersections = 500;
		m_Renderer->renderSettings.displayBVH = false;

		// The FRAME_WIDTH and FRAME_HEIGHT define the dimensions of the render frame.
		// These values represent the actual number of pixels that the renderer will process to produce the final image.
		// Note: These dimensions are different from the size or aspect ratio of the ImGui viewport window in the editor.
		// The camera's aspect ratio only stretches the image to fit the viewport window correctly
		#define FRAME_WIDTH 1200.0f
		#define FRAME_HEIGHT 800.0f
		m_Renderer->SetFrameResolution(glm::vec2(FRAME_WIDTH, FRAME_HEIGHT));

		/// ------------- SCRIPTING -------------- /// (just to see how to use the system) will change in the future
		/* 
		 Test SCRIPT (will be in its own file in the future)
		class TestScript : public Script
		{
			virtual void OnCreate() override 
			{
				LR_EDITOR_INFO("TestScript::OnCreate (called in scene.OnStart())");
			}
		
			virtual void OnUpdate() override
			{
				LR_EDITOR_INFO("TestScript::OnUpdate (this should get called every frame)");
			}
		
			virtual void OnDestroy() override
			{
				LR_EDITOR_INFO("TestScript::OnDestroy (called once the entity gets destroyed)");
			}
		};
		 Testing the TEST SCRIPT on a TEST ENTITY
		Entity testEntity = m_Scene->CreateEntity();
		{
			std::string& tag = testEntity.GetComponent<TagComponent>().Tag;
			tag = std::string("TestScript");
		}
		testEntity.AddComponent<ScriptComponent>(new TestScript());
		testEntity.RemoveComponent<ScriptComponent>();
		m_Scene->DestroyEntity(testEntity); // testing the script's OnDestroy() function
		*/
		/// ---------------------------------------- ///

		m_Scene->OnStart(); // calls the onStart() of the scripts
		
		m_Renderer->Init();
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
				if (ImGui::MenuItem("Themes")) { m_EditorState->temp.ThemeSettingsPanelOpen = true; }
				ImGui::EndMenu();
			}

			ImGui::EndMainMenuBar();
		}

		m_SceneHierarchyPanel.OnImGuiRender(m_Scene);
		m_InspectorPanel.OnImGuiRender(m_Scene);
		if (m_EditorState->temp.ThemeSettingsPanelOpen) { m_ThemesPanel.OnImGuiRender(); }

		// Parse the scene for rendering
		std::shared_ptr<RenderableScene> rScene = m_SceneManager->ParseSceneForRendering(m_Scene);
		
		// Render The Scene
		m_Renderer->SubmitScene(rScene);
		std::shared_ptr<IImage2D> RenderedFrame = m_Renderer->RenderScene();
		m_ViewportPanel.OnImGuiRender(RenderedFrame, m_EditorState);		
	}

	void EditorLayer::onDetach()
	{
		m_Scene->OnShutdown();
		serializeState(m_EditorState); // save the persistent part of the editor state to the file
	}
}