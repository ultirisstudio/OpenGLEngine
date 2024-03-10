#include "depch.h"
#include "ColliderComponent.h"

#include <reactphysics3d/reactphysics3d.h>

#include <OpenGLEngine/Entity/Entity.h>
#include <OpenGLEngine/Physic/PhysicEngine.h>
#include <OpenGLEngine/Entity/Components/TransformComponent.h>

namespace OpenGLEngine
{
	ColliderComponent::ColliderComponent() : collisionBody(nullptr), collider(nullptr), lastPosition(glm::vec3(0.0f)), lastRotation(glm::vec3(0.0f)), lastScale(glm::vec3(0.0f))
	{
	}

	void ColliderComponent::Init()
	{
		glm::vec3 position = Component::entity->GetComponent<TransformComponent>().Position;
		glm::vec3 rotation = Component::entity->GetComponent<TransformComponent>().Rotation;
		glm::vec3 scale = Component::entity->GetComponent<TransformComponent>().Scale;

		reactphysics3d::Transform transform(reactphysics3d::Vector3(position.x, position.y, position.z), reactphysics3d::Quaternion::fromEulerAngles(rotation.x, rotation.y, rotation.z));

		//collisionBody = PhysicEngine::GetPhysicWorld()->createCollisionBody(transform);

		const reactphysics3d::Vector3 halfExtents(scale.x, scale.y, scale.z);
		reactphysics3d::BoxShape* boxShape = PhysicEngine::GetPhysicsCommon()->createBoxShape(halfExtents);

		//collider = collisionBody->addCollider(boxShape, reactphysics3d::Transform::identity());

		rigidbody = PhysicEngine::GetPhysicWorld()->createRigidBody(transform);
		rigidbody->setType(reactphysics3d::BodyType::STATIC);
		rigidbody->enableGravity(false);

		collider = rigidbody->addCollider(boxShape, reactphysics3d::Transform::identity());
	}

	void ColliderComponent::Update()
	{
		if (collisionBody)
		{
			glm::vec3 position = Component::entity->GetComponent<TransformComponent>().Position;
			glm::vec3 rotation = Component::entity->GetComponent<TransformComponent>().Rotation;
			glm::vec3 scale = Component::entity->GetComponent<TransformComponent>().Scale;

			if (position != lastPosition)
			{
				const reactphysics3d::Vector3 halfExtents(scale.x, scale.y, scale.z);
				reactphysics3d::BoxShape* boxShape = PhysicEngine::GetPhysicsCommon()->createBoxShape(halfExtents);
				rigidbody->setTransform(reactphysics3d::Transform(reactphysics3d::Vector3(position.x, position.y, position.z), reactphysics3d::Quaternion::identity()));
				//collisionBody->setTransform(reactphysics3d::Transform(reactphysics3d::Vector3(position.x, position.y, position.z), reactphysics3d::Quaternion::identity()));
				lastPosition = position;
			}

			if (rotation != lastRotation)
			{
				const reactphysics3d::Vector3 halfExtents(scale.x, scale.y, scale.z);
				reactphysics3d::BoxShape* boxShape = PhysicEngine::GetPhysicsCommon()->createBoxShape(halfExtents);
				rigidbody->setTransform(reactphysics3d::Transform(reactphysics3d::Vector3(position.x, position.y, position.z), reactphysics3d::Quaternion::fromEulerAngles(rotation.x, rotation.y, rotation.z)));
				rigidbody->removeCollider(collider);
				collider = rigidbody->addCollider(boxShape, reactphysics3d::Transform::identity());
				//collisionBody->setTransform(reactphysics3d::Transform(reactphysics3d::Vector3(position.x, position.y, position.z), reactphysics3d::Quaternion::fromEulerAngles(rotation.x, rotation.y, rotation.z)));
				lastRotation = rotation;
			}

			if (scale != lastScale)
			{
				const reactphysics3d::Vector3 halfExtents(scale.x, scale.y, scale.z);
				reactphysics3d::BoxShape* boxShape = PhysicEngine::GetPhysicsCommon()->createBoxShape(halfExtents);
				//collisionBody->removeCollider(collider);
				//collider = collisionBody->addCollider(boxShape, reactphysics3d::Transform::identity());
				rigidbody->removeCollider(collider);
				collider = rigidbody->addCollider(boxShape, reactphysics3d::Transform::identity());
				lastScale = scale;
			}
		}
	}
}