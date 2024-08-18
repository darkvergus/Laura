#pragma once
#include "entt/entt.hpp"

namespace Laura
{

	class Entity
	{
	public:
		Entity() = default;
		Entity(entt::entity entity, entt::registry* registry);
		~Entity();

		template<typename T, typename... Args>
		T& AddComponent(Args&&... args)
		{
			return m_Registry->emplace<T>(m_EntityID, std::forward<Args>(args)...);
		}

		template<typename T>
		T& GetComponent() const
		{
			return m_Registry->get<T>(m_EntityID);
		}

		template<typename T>
		bool HasComponent()
		{
			return m_Registry->all_of<T>(m_EntityID);
		}

		template<typename T>
		void RemoveComponent()
		{
			m_Registry->remove<T>(m_EntityID);
		}

		inline entt::entity GetID() const { return m_EntityID; }

	private:
		entt::entity m_EntityID{ entt::null };
		// using a raw pointer because the registry is owned by the scene
		// only the scene should be able to delete the registry
		entt::registry* m_Registry = nullptr; 
	};

}