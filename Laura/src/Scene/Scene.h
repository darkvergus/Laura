#pragma once
#include "entt/entt.hpp"

#include "Scene/Entity.h"
#include "Scene/Skybox.h"
#include "Scene/Components.h"

namespace Laura
{
	class Scene
	{
	public:
		LR_GUID skyboxGuid;
	public:
		Scene();
		~Scene();
		Entity& CreateEntity();
		void DestroyEntity(const Entity& entity);

		void OnStart();
		void OnUpdate();
		void OnShutdown();

		inline entt::registry* GetRegistry() const { return m_Registry; }

	private:
		entt::registry* m_Registry;
	};
}