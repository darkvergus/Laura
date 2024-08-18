#include "EditorLayer.h"

namespace Laura
{

	EditorLayer::EditorLayer(std::shared_ptr<Renderer> renderer)
		: m_Renderer(renderer),
		prevViewportWindowSize(glm::vec2(0.0f, 0.0f)),
		prevViewportWindowPos(glm::vec2(0.0f, 0.0f)),
		aspectRatio(16.0f/9.0f)
	{
		setLayerName("EditorLayer");
	}

	void EditorLayer::onAttach()
	{
		// Setting up the Skybox
		m_Scene.skybox.changeType(SkyboxType::SKYBOX_TEXTURE);
		std::string texturePath = LR_RESOURCES_PATH "Skyboxes/Metro_default.hdr";
		m_Scene.skybox.setTexturePath(texturePath);


		// Adding a CAMERA to the scene
		Entity camera = m_Scene.AddEntity();
		TransformComponent& cameraTransform = camera.AddComponent<TransformComponent>();
		TransformHandler::Translate(cameraTransform, { 0.0f, 40.0f, -200.0f });
		CameraComponent cameraComponent = camera.AddComponent<CameraComponent>();
		// Using the default values for the camera component
		//cameraComponent.fov = 80.0f;
		//cameraComponent.aspectRatio = 16.0f/9.0f;


		// Adding a 3D MODEL to the scene
		Entity dragon = m_Scene.AddEntity();
		MeshComponent& dragonMesh			= dragon.AddComponent<MeshComponent>();
		TransformComponent& dragonTransform	= dragon.AddComponent<TransformComponent>();
		MaterialComponent& dragonMaterial	= dragon.AddComponent<MaterialComponent>();
		//TransformHandler::Translate(dragonTransform, { 0.0f, 0.0f, 0.0f });

		dragonMesh.mesh = MeshLoader::loadMesh(std::string(LR_RESOURCES_PATH "Models/stanford_dragon_pbr.glb"));


		// The FRAME_WIDTH and FRAME_HEIGHT define the dimensions of the render frame.
		// These values represent the actual number of pixels that the renderer will process to produce the final image.
		// Note: These dimensions are different from the size or aspect ratio of the ImGui viewport window in the editor.
		// The camera's aspect ratio only stretches the image to fit the viewport window correctly
		#define FRAME_WIDTH 1920.0f/5.0f
		#define FRAME_HEIGHT 1080.0f/5.0f
		m_Renderer->renderSettings.frameDimentions = glm::vec2(FRAME_WIDTH, FRAME_HEIGHT);
		m_Renderer->renderSettings.raysPerPixel = 1;
		m_Renderer->renderSettings.bouncesPerRay = 5;
		m_Renderer->renderSettings.maxAABBIntersections = 10000;
		m_Renderer->renderSettings.displayBVH = false;

		m_Renderer->BeginScene(camera, m_Scene.skybox);
		MeshComponent dragon_mesh = dragon.GetComponent<MeshComponent>();
		TransformComponent dragon_transform = dragon.GetComponent<TransformComponent>();
		MaterialComponent dragon_material = dragon.GetComponent<MaterialComponent>();
		m_Renderer->Submit(dragon_mesh, dragon_transform, dragon_material);



		//Old API
		//m_Camera.transform.setPosition({ 0.0f, 40.0f, -200.0f });
		//m_Camera.transform.setRotation({ 0.0f, 0.0f, 0.0f });
		//m_Camera.setFOV(80.0f);
		//
		//m_Environment.skybox.changeType(SkyboxType::SKYBOX_TEXTURE);
		//std::string texturePath = LR_RESOURCES_PATH "Skyboxes/Metro_default.hdr";
		//m_Environment.skybox.setTexturePath(texturePath);
		//
		//m_Renderer->renderSettings.viewportDimensions = glm::vec2(300, 100);
		//m_Renderer->renderSettings.raysPerPixel = 1;
		//m_Renderer->renderSettings.bouncesPerRay = 5;
		//m_Renderer->renderSettings.maxAABBIntersections = 10000;
		//m_Renderer->renderSettings.displayBVH = false;
		//
		//m_Renderer->BeginScene(m_Camera, m_Environment);
		//
		//MeshComponent model3D = MeshLoader::loadMesh(std::string(LR_RESOURCES_PATH "Models/stanford_dragon_pbr.glb"));
		////MeshComponent model3D = MeshLoader::loadMesh(std::string(APP_RESOURCES_PATH "models/suzanne_high_poly_rotated.glb"));
		////MeshComponent model3D = MeshLoader::loadMesh(std::string(APP_RESOURCES_PATH "models/tiny_house.glb"));
		////MeshComponent model3D = MeshLoader::loadMesh(std::string(APP_RESOURCES_PATH "models/sponza.obj"));
		////MeshComponent model3D = MeshLoader::loadMesh(std::string(APP_RESOURCES_PATH "models/stanford_bunny.obj"));
		//m_Renderer->SubmitMesh(model3D);
	}

	void EditorLayer::onDetach()
	{
	}

	void EditorLayer::onUpdate()
	{
	}

	void EditorLayer::onImGuiRender()
	{
		bool showDemoWindow = true;
		ImGui::ShowDemoWindow(&showDemoWindow);

		m_Renderer->renderSettings.accumulateFrames = true;

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

			m_Renderer->renderSettings.frameDimentions = glm::vec2(viewportSize.x, viewportSize.y);
			m_Renderer->renderSettings.accumulateFrames = false;
			m_Renderer->UpdateRenderSettingsUBO();

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
	}

	void EditorLayer::onEvent(Event* event)
	{
	}
}