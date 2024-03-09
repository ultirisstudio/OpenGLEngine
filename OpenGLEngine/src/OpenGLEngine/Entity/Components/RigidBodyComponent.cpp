#include "depch.h"
#include "RigidBodyComponent.h"

#include <reactphysics3d/reactphysics3d.h>
#include <glm/glm.hpp>

#include <OpenGLEngine/Entity/Entity.h>
#include <OpenGLEngine/Physic/PhysicEngine.h>
#include <OpenGLEngine/Entity/Components/TransformComponent.h>

namespace OpenGLEngine
{
	RigidBodyComponent::RigidBodyComponent()
	{
		
	}

	void RigidBodyComponent::Init()
	{
		if (Component::entity)
		{
			if (Component::entity->HasComponent<TransformComponent>())
			{
				auto& tc = Component::entity->GetComponent<TransformComponent>();
				glm::vec3 entityPosition = tc.Position;
				glm::vec3 entityRotation = tc.Rotation;

				reactphysics3d::Vector3 position(entityPosition.x, entityPosition.y, entityPosition.z);
				reactphysics3d::Vector3 rotation(entityRotation.x, entityRotation.y, entityRotation.z);

				reactphysics3d::Quaternion orientation = reactphysics3d::Quaternion::fromEulerAngles(rotation);

				reactphysics3d::Transform transform(position, orientation);

				rigidbody = PhysicEngine::GetPhysicWorld()->createRigidBody(transform);
			}
		}
	}
}