#include "Core/Layers/SceneLayer.h"
#include "Core/Events/SceneEvents.h"

namespace Laura
{
	SceneLayer::SceneLayer(std::shared_ptr<IEventDispatcher> eventDispatcher)
		: m_EventDispatcher(eventDispatcher), m_Scene(std::make_shared<Scene>()) {
	}

	void SceneLayer::onAttach() {
	}

	void SceneLayer::onDetach() {
	}

	void SceneLayer::onUpdate() {
		m_Scene->OnUpdate();
	}

	void SceneLayer::onImGuiRender() {
	}

	void SceneLayer::onEvent(std::shared_ptr<IEvent> event) {
		if (event->GetType() == EventType::SCENE_CREATE_EVENT) {
			m_Scene = std::make_shared<Scene>(); // create new scene

			EntityHandle camera = m_Scene->CreateEntity();
			camera.GetComponent<TagComponent>().Tag = std::string("Camera");
			camera.AddComponent<TransformComponent>().SetTranslation({ 0.0f, 40.0f, -200.0f });
			camera.AddComponent<CameraComponent>().fov = 30.0f;
				
			EntityHandle dragon = m_Scene->CreateEntity();
			dragon.GetComponent<TagComponent>().Tag = "Dragon";
			dragon.AddComponent<TransformComponent>(); // Add if you want to set translation/scale
			dragon.AddComponent<MaterialComponent>();
			//auto& meshComponent = dragon.AddComponent<MeshComponent>();
			//meshComponent.guid = m_AssetManager->LoadAsset(EDITOR_RESOURCES_PATH "Models/stanford_dragon_pbr.glb");
			//meshComponent.sourceName = "stanford_dragon_pbr.glb";

			EntityHandle bunny = m_Scene->CreateEntity();
			bunny.GetComponent<TagComponent>().Tag = "Bunny";
			bunny.AddComponent<TransformComponent>();
			bunny.AddComponent<MaterialComponent>();
			//auto& meshComponent = bunny.AddComponent<MeshComponent>();
			//meshComponent.guid = m_AssetManager->LoadAsset(EDITOR_RESOURCES_PATH "Models/stanford_bunny_pbr.glb");
			//meshComponent.sourceName = "stanford_bunny_pbr.glb";

			m_EventDispatcher->dispatchEvent(std::make_shared<SceneLoadedEvent>(m_Scene));
		}

		else if (event->GetType() == EventType::SCENE_CLOSE_EVENT) {
			m_Scene = nullptr;
		}
	}
}