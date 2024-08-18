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
		Skybox skybox;
	public:
		Scene();
		~Scene();
		Entity AddEntity();
		void DestroyEntity(const Entity& entity);

		inline entt::registry* Get() const { return m_Registry; }

	private:
		entt::registry* m_Registry;
	};
}