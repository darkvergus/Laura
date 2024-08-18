#include "Scene/Scene.h"

namespace Laura
{
	Scene::Scene()
	{
		m_Registry = new entt::registry();
	}

	Scene::~Scene()
	{
		delete m_Registry;
	}

	Entity Scene::AddEntity()
	{
		// constructs a naked entity with no components and returns its identifier
		entt::entity entity = m_Registry->create();
		return Entity(entity, m_Registry); 
	}

	void Scene::DestroyEntity(const Entity& entity)
	{
		m_Registry->destroy(entity.GetID());
	}
}
