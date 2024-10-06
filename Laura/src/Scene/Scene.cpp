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

	Entity& Scene::CreateEntity()
	{
		// constructs a naked entity with no components and returns its identifier
		entt::entity entityID = m_Registry->create();
		Entity entity(entityID, m_Registry);
		entity.AddComponent<TagComponent>("EmptyEntity");
		m_Registry->remove<TagComponent>(entityID);
		entity.AddComponent<TagComponent>("Empty Entity");
		return entity;
	}

	void Scene::DestroyEntity(const Entity& entity)
	{
		if (entity.HasComponent<ScriptComponent>())
		{
			entity.GetComponent<ScriptComponent>().script->OnDestroy();
		}
		m_Registry->destroy(entity.GetID());
		LR_CORE_INFO("Destroyed entity");
	}

	void Scene::OnStart()
	{
		{
			auto view = m_Registry->view<ScriptComponent>();
			for (auto entity : view)
			{
				view.get<ScriptComponent>(entity).script->OnCreate();
			}
		}
	}

	void Scene::OnUpdate()
	{
		/// Finding the MAIN CAMERA ///
		CameraComponent* mainCamera = nullptr;
		TransformComponent* mainCameraTransform = nullptr;
		{
			auto group = m_Registry->group<CameraComponent, TransformComponent>();
			for (auto entity : group)
			{
				auto [camera, transform] = group.get<CameraComponent, TransformComponent>(entity);
				if (camera.isMain)
				{
					mainCamera = &camera;
					mainCameraTransform = &transform;
					break;
				}
			}
		}

		/// Executing all the SCRIPTS ///
		{
			auto view = m_Registry->view<ScriptComponent>();
			for (auto entity : view)
			{
				view.get<ScriptComponent>(entity).script->OnUpdate();
			}
		}
	}

	void Scene::OnShutdown()
	{
	}
}
