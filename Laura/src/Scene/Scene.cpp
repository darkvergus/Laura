#include "Scene.h"

namespace Laura
{

	Scene::Scene() {
		m_Registry = new entt::registry();
	}

	Scene::~Scene() {
		delete m_Registry;
	}

	EntityHandle Scene::CreateEntity() {
		entt::entity entityID = m_Registry->create();
		EntityHandle entity(entityID, m_Registry);
		entity.AddComponent<TagComponent>("Empty Entity");
		entity.AddComponent<GUIDComponent>();
		return entity;
	}

	void Scene::DestroyEntity(EntityHandle entity) {
		m_Registry->destroy(entity.GetID());
	}

	void Scene::OnStart() {
	}

	void Scene::OnUpdate() {
	}

	void Scene::OnShutdown() {
	}

	bool Laura::Scene::Serialize(const std::filesystem::path& filepath) {
		std::cout << "Serializing: " << filepath.string() << std::endl;
		return false;
	}

	bool Laura::Scene::Deserialize(const std::filesystem::path& filepath) {
		std::cout << "Deserializing: " << filepath.string() << std::endl;
		return false;
	}
}
