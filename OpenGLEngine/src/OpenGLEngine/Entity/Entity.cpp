#include "depch.h"
#include "Entity.h"

//#include <OpenGLEngine/Entity/Components/Physics/RigidBodyComponent.h>

namespace OpenGLEngine
{
	Entity::Entity(entt::entity handle, Scene* scene)
		: m_EntityHandle(handle), m_Scene(scene)
	{
	}

	/*void Entity::Destroy()
	{
		if (HasComponent<OpenGLEngine::RigidBodyComponent>())
		{
			GetComponent<OpenGLEngine::RigidBodyComponent>().Destroy();
		}
	}*/
}