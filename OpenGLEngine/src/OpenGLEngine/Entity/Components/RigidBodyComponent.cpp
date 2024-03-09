#include "depch.h"
#include "RigidBodyComponent.h"

#include <reactphysics3d/reactphysics3d.h>
#include <reactphysics3d/decimal.h>

#include <glm/glm.hpp>

#include <OpenGLEngine/Entity/Entity.h>
#include <OpenGLEngine/Physic/PhysicEngine.h>
#include <OpenGLEngine/Entity/Components/TransformComponent.h>

namespace OpenGLEngine
{
	RigidBodyComponent::RigidBodyComponent() : rigidbody(nullptr)
	{
		
	}

	void RigidBodyComponent::Init()
	{
		auto& tc = Component::entity->GetComponent<TransformComponent>();
		glm::vec3 entityPosition = tc.Position;
		glm::vec3 entityRotation = tc.Rotation;

		lastPosition = entityPosition;
		lastRotation = entityRotation;

		reactphysics3d::Vector3 position(entityPosition.x, entityPosition.y, entityPosition.z);
		reactphysics3d::Vector3 rotation(entityRotation.x, entityRotation.y, entityRotation.z);

		reactphysics3d::Quaternion orientation = reactphysics3d::Quaternion::fromEulerAngles(rotation);

		reactphysics3d::Transform transform(position, orientation);

		rigidbody = PhysicEngine::GetPhysicWorld()->createRigidBody(transform);
		rigidbody->setType(reactphysics3d::BodyType::DYNAMIC);
		//rigidbody->enableGravity(true);
		//rigidbody->setIsActive(true);
	}

	void RigidBodyComponent::Update()
	{
		auto& tc = Component::entity->GetComponent<TransformComponent>();

		const reactphysics3d::Transform& transform = rigidbody->getTransform();

		const reactphysics3d::Vector3& position = transform.getPosition();
		const reactphysics3d::Vector3& rotation = transform.getOrientation().getVectorV();

		tc.Position = glm::vec3(position.x, position.y, position.z);
		tc.Rotation = glm::vec3(rotation.x, rotation.y, rotation.z);

		/*reactphysics3d::decimal delta_pos_x = position.x - lastPosition.x;
		reactphysics3d::decimal delta_pos_y = position.y - lastPosition.y;
		reactphysics3d::decimal delta_pos_z = position.z - lastPosition.z;

		reactphysics3d::decimal delta_rot_x = rotation.x - lastPosition.x;
		reactphysics3d::decimal delta_rot_y = rotation.y - lastPosition.y;
		reactphysics3d::decimal delta_rot_z = rotation.z - lastPosition.z;

		tc.Position.x += delta_pos_x;
		tc.Position.y += delta_pos_y;
		tc.Position.z += delta_pos_z;

		lastPosition = tc.Position;
		lastRotation = tc.Rotation;*/
	}
}