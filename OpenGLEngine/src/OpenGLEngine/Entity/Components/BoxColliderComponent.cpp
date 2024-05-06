#include "depch.h"
#include "BoxColliderComponent.h"

#include <reactphysics3d/reactphysics3d.h>

#include <OpenGLEngine/Entity/Entity.h>
#include <OpenGLEngine/Physic/PhysicEngine.h>
#include <OpenGLEngine/Entity/Components/TransformComponent.h>
#include <OpenGLEngine/Entity/Components/RigidBodyComponent.h>

namespace OpenGLEngine
{
	BoxColliderComponent::BoxColliderComponent() : collider(nullptr), rigidbody(nullptr)
	{
	}

	BoxColliderComponent::~BoxColliderComponent()
	{
		rigidbody->removeCollider(collider);
	}

	void BoxColliderComponent::Init()
	{
		auto& tc = Component::entity->GetComponent<TransformComponent>();
		glm::vec3 entityPosition = tc.Position;
		glm::vec3 entityRotation = tc.Rotation;
		glm::vec3 entityScale = tc.Scale;

		auto& rbc = Component::entity->GetComponent<RigidBodyComponent>();
		rigidbody = rbc.GetRigidBody();

		const reactphysics3d::Vector3 halfExtents(entityScale.x, entityScale.y, entityScale.z);
		reactphysics3d::BoxShape* boxShape = PhysicEngine::GetPhysicsCommon()->createBoxShape(halfExtents);

		collider = rigidbody->addCollider(boxShape, reactphysics3d::Transform::identity());
	}
}