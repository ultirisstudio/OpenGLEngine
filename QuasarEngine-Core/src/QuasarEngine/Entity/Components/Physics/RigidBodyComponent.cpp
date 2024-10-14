#include "qepch.h"
#include "RigidBodyComponent.h"

#include <reactphysics3d/reactphysics3d.h>
#include <reactphysics3d/decimal.h>

#include <glm/glm.hpp>

#include <QuasarEngine/Entity/Entity.h>
#include <QuasarEngine/Physic/PhysicEngine.h>
#include <QuasarEngine/Entity/Components/TransformComponent.h>

namespace QuasarEngine
{
	RigidBodyComponent::RigidBodyComponent() : rigidbody(nullptr), collider(nullptr), bodyType(reactphysics3d::BodyType::DYNAMIC), enableGravity(true), bodyTypeString("DYNAMIC")
	{
		
	}

	RigidBodyComponent::~RigidBodyComponent()
	{
		
	}

	void RigidBodyComponent::Destroy()
	{
		if (rigidbody)
		{
			PhysicEngine::GetPhysicWorld()->destroyRigidBody(rigidbody);
		}
	}

	void RigidBodyComponent::Init()
	{
		Entity entity{ entt_entity, registry };
		auto& tc = entity.GetComponent<TransformComponent>();
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
	}

	void RigidBodyComponent::Update()
	{
		Entity entity{ entt_entity, registry };
		auto& tc = entity.GetComponent<TransformComponent>();

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

	void RigidBodyComponent::UpdateLinearAxisFactor()
	{
		rigidbody->setLinearLockAxisFactor(reactphysics3d::Vector3(m_LinearAxisFactorX, m_LinearAxisFactorY, m_LinearAxisFactorZ));
	}

	void RigidBodyComponent::UpdateAngularAxisFactor()
	{
		rigidbody->setAngularLockAxisFactor(reactphysics3d::Vector3(m_AngularAxisFactorX, m_AngularAxisFactorY, m_AngularAxisFactorZ));
	}
}