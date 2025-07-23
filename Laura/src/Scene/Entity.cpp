#include "Entity.h"

namespace Laura
{

	EntityHandle::EntityHandle(entt::entity entity, entt::registry* registry)
		: m_EntityID(entity), m_Registry(registry)
	{
	}

	EntityHandle::~EntityHandle()
	{
	}
}