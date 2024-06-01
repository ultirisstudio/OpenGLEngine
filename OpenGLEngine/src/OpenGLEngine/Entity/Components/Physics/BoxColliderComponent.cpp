#include "depch.h"
#include "BoxColliderComponent.h"

#include <reactphysics3d/reactphysics3d.h>

#include <OpenGLEngine/Entity/Entity.h>
#include <OpenGLEngine/Physic/PhysicEngine.h>
#include <OpenGLEngine/Entity/Components/TransformComponent.h>
#include <OpenGLEngine/Entity/Components/Physics/RigidBodyComponent.h>

namespace OpenGLEngine
{
	BoxColliderComponent::BoxColliderComponent()
	{
	}

	BoxColliderComponent::~BoxColliderComponent()
	{
		if (entity->HasComponent<RigidBodyComponent>())
		{
			auto& rb = entity->GetComponent<RigidBodyComponent>();
			rb.GetRigidBody()->removeCollider(m_Collider);
		}
	}

	void BoxColliderComponent::Init()
	{
		auto& tc = Component::entity->GetComponent<TransformComponent>();
		glm::vec3 entityScale = tc.Scale;

		const reactphysics3d::Vector3 halfExtents(entityScale.x, entityScale.y, entityScale.z);
		reactphysics3d::BoxShape* boxShape = PhysicEngine::GetPhysicsCommon()->createBoxShape(halfExtents);

		if (entity->HasComponent<RigidBodyComponent>())
		{
			auto& rb = entity->GetComponent<RigidBodyComponent>();
			m_Collider = rb.GetRigidBody()->addCollider(boxShape, reactphysics3d::Transform::identity());
		}
	}

	void BoxColliderComponent::UpdateColliderMaterial()
	{
		m_Collider->getMaterial().setMassDensity(mass);
		m_Collider->getMaterial().setBounciness(bounciness);
		m_Collider->getMaterial().setFrictionCoefficient(friction);
	}

	void BoxColliderComponent::UpdateColliderSize()
	{
		if (entity->HasComponent<RigidBodyComponent>())
		{
			auto& rb = entity->GetComponent<RigidBodyComponent>();
			rb.GetRigidBody()->removeCollider(m_Collider);

			reactphysics3d::BoxShape* boxShape = nullptr;

			if (m_UseEntityScale)
			{
				auto& tc = Component::entity->GetComponent<TransformComponent>();
				const reactphysics3d::Vector3 halfExtents(tc.Scale.x, tc.Scale.y, tc.Scale.z);
				boxShape = PhysicEngine::GetPhysicsCommon()->createBoxShape(halfExtents);
			}
			else
			{
				const reactphysics3d::Vector3 halfExtents(m_Size.x, m_Size.y, m_Size.z);
				boxShape = PhysicEngine::GetPhysicsCommon()->createBoxShape(halfExtents);
			}

			if (boxShape)
				m_Collider = rb.GetRigidBody()->addCollider(boxShape, reactphysics3d::Transform::identity());
		}
	}
}