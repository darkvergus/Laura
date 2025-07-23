#pragma once
#include "entt/entt.hpp"

#include "Scene/Entity.h"
#include "Scene/Components.h"

namespace Laura
{
	class Scene
	{
	public:
		Scene();
		~Scene();
		EntityHandle CreateEntity();
		void DestroyEntity(EntityHandle entity);

		void OnStart();
		void OnUpdate();
		void OnShutdown();

		inline entt::registry* GetRegistry() const { return m_Registry; }

	private:
		entt::registry* m_Registry;
	};
}