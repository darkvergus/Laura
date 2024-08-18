#include "Entity.h"

namespace Laura
{

	Entity::Entity(entt::entity entity, entt::registry* registry)
		: m_EntityID(entity), m_Registry(registry)
	{
	}

	Entity::~Entity()
	{
	}
}