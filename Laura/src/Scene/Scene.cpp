#include "Scene/Scene.h"

namespace Laura
{

	Scene::Scene() {
		m_Registry = new entt::registry();
	}

	Scene::~Scene() {
		delete m_Registry;
	}

	Entity& Scene::CreateEntity() {
		entt::entity entityID = m_Registry->create();
		Entity entity(entityID, m_Registry);
		entity.AddComponent<TagComponent>("Empty Entity");
		entity.AddComponent<GUIDComponent>();
		return entity;
	}

	void Scene::DestroyEntity(const Entity& entity) {
		m_Registry->destroy(entity.GetID());
		LOG_ENGINE_INFO("Destroyed entity");
	}

	void Scene::OnStart() {
	}

	void Scene::OnUpdate() {
	}

	void Scene::OnShutdown() {
	}
}
