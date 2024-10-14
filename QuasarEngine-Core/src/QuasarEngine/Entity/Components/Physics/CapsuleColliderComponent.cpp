#include "qepch.h"
#include "CapsuleColliderComponent.h"

#include <reactphysics3d/reactphysics3d.h>

#include <QuasarEngine/Entity/Entity.h>
#include <QuasarEngine/Physic/PhysicEngine.h>
#include <QuasarEngine/Entity/Components/TransformComponent.h>
#include <QuasarEngine/Entity/Components/Physics/RigidBodyComponent.h>

namespace QuasarEngine
{
	CapsuleColliderComponent::CapsuleColliderComponent()
	{
	}

	CapsuleColliderComponent::~CapsuleColliderComponent()
	{
		Entity entity{entt_entity, registry };
		if (entity.HasComponent<RigidBodyComponent>())
		{
			auto& rb = entity.GetComponent<RigidBodyComponent>();
			rb.GetRigidBody()->removeCollider(m_Collider);
		}
	}

	void CapsuleColliderComponent::Init()
	{
		Entity entity{entt_entity, registry };
		auto& tc = entity.GetComponent<TransformComponent>();
		glm::vec3 entityScale = tc.Scale;

		reactphysics3d::CapsuleShape* capsuleShape = PhysicEngine::GetPhysicsCommon()->createCapsuleShape(m_Radius, m_Height);
		
		if (entity.HasComponent<RigidBodyComponent>())
		{
			auto& rb = entity.GetComponent<RigidBodyComponent>();
			m_Collider = rb.GetRigidBody()->addCollider(capsuleShape, reactphysics3d::Transform::identity());
		}
	}

	void CapsuleColliderComponent::UpdateColliderMaterial()
	{
		m_Collider->getMaterial().setMassDensity(mass);
		m_Collider->getMaterial().setBounciness(bounciness);
		m_Collider->getMaterial().setFrictionCoefficient(friction);
	}

	void CapsuleColliderComponent::UpdateColliderSize()
	{
		Entity entity{entt_entity, registry };
		if (entity.HasComponent<RigidBodyComponent>())
		{
			auto& rb = entity.GetComponent<RigidBodyComponent>();
			rb.GetRigidBody()->removeCollider(m_Collider);

			reactphysics3d::CapsuleShape* capsuleShape = PhysicEngine::GetPhysicsCommon()->createCapsuleShape(m_Radius, m_Height);

			if (capsuleShape)
				m_Collider = rb.GetRigidBody()->addCollider(capsuleShape, reactphysics3d::Transform::identity());
		}
	}
}