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
	RigidBodyComponent::RigidBodyComponent() : rigidbody(nullptr), collider(nullptr), bodyType(reactphysics3d::BodyType::DYNAMIC), enableGravity(true), mass(1.0f), friction(0.3f), bounciness(0.3f), bodyTypeString("DYNAMIC")
	{
		
	}

	RigidBodyComponent::~RigidBodyComponent()
	{
		rigidbody->removeCollider(collider);
		delete rigidbody;
	}

	void RigidBodyComponent::Init()
	{
		auto& tc = Component::entity->GetComponent<TransformComponent>();
		glm::vec3 entityPosition = tc.Position;
		glm::vec3 entityRotation = tc.Rotation;
		glm::vec3 entityScale = tc.Scale;

		reactphysics3d::Vector3 position(entityPosition.x, entityPosition.y, entityPosition.z);
		reactphysics3d::Vector3 rotation(entityRotation.x, entityRotation.y, entityRotation.z);

		reactphysics3d::Quaternion orientation = reactphysics3d::Quaternion::fromEulerAngles(rotation);

		reactphysics3d::Transform transform(position, orientation);

		rigidbody = PhysicEngine::GetPhysicWorld()->createRigidBody(transform);
		rigidbody->setType(bodyType);
		rigidbody->enableGravity(enableGravity);
		rigidbody->setIsDebugEnabled(true);

		const reactphysics3d::Vector3 halfExtents(entityScale.x, entityScale.y, entityScale.z);
		reactphysics3d::BoxShape* boxShape = PhysicEngine::GetPhysicsCommon()->createBoxShape(halfExtents);

		collider = rigidbody->addCollider(boxShape, reactphysics3d::Transform::identity());
	}

	void RigidBodyComponent::Update()
	{
		auto& tc = Component::entity->GetComponent<TransformComponent>();

		const reactphysics3d::Transform& transform = rigidbody->getTransform();

		const reactphysics3d::Vector3& position = transform.getPosition();
		const reactphysics3d::Quaternion& orientation = transform.getOrientation();

		tc.Position = glm::vec3(position.x, position.y, position.z);

		glm::quat rotation = glm::quat(orientation.w, orientation.x, orientation.y, orientation.z);
		tc.Rotation = glm::eulerAngles(rotation);
	}

	void RigidBodyComponent::UpdateEnableGravity()
	{
		rigidbody->enableGravity(enableGravity);
	}

	void RigidBodyComponent::UpdateBodyType()
	{
		if (strcmp(bodyTypeString.c_str(), "DYNAMIC") == 0)
		{
			rigidbody->setType(reactphysics3d::BodyType::DYNAMIC);
		}
		else if (strcmp(bodyTypeString.c_str(), "STATIC") == 0)
		{
			rigidbody->setType(reactphysics3d::BodyType::STATIC);
		}
		else if (strcmp(bodyTypeString.c_str(), "KINEMATIC") == 0)
		{
			rigidbody->setType(reactphysics3d::BodyType::KINEMATIC);
		}
		else
		{
			std::cout << "Invalid body type" << std::endl;
		}
	}

	void RigidBodyComponent::UpdateMaterial()
	{
		collider->getMaterial().setMassDensity(mass);
		collider->getMaterial().setBounciness(bounciness);
		collider->getMaterial().setFrictionCoefficient(friction);
	}
}