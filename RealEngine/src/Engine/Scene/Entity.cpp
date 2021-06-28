
#include "repch.h"
#include "Entity.h"

namespace RealEngine {

	Entity::Entity(entt::entity id, Scene* scene)
		: m_EntityId(id), m_Scene(scene)
	{

	}

	Entity::~Entity()
	{

	}
}