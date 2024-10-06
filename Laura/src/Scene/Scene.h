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
		std::shared_ptr<Skybox> skybox;
	public:
		Scene();
		~Scene();
		Entity& CreateEntity();
		void DestroyEntity(const Entity& entity);

		void OnStart();
		void OnUpdate();
		void OnShutdown();

		inline entt::registry* Get() const { return m_Registry; }

	private:
		entt::registry* m_Registry;
	};
}