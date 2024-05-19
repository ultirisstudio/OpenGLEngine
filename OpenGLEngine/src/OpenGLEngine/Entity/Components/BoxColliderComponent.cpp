#include "depch.h"
#include "BoxColliderComponent.h"

#include <reactphysics3d/reactphysics3d.h>

#include <OpenGLEngine/Entity/Entity.h>
#include <OpenGLEngine/Physic/PhysicEngine.h>
#include <OpenGLEngine/Entity/Components/TransformComponent.h>
#include <OpenGLEngine/Entity/Components/RigidBodyComponent.h>

namespace OpenGLEngine
{
	BoxColliderComponent::BoxColliderComponent() : m_Collider(nullptr), mass(1.0f), friction(0.3f), bounciness(0.3f)
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
		glm::vec3 entityPosition = tc.Position;
		glm::vec3 entityRotation = tc.Rotation;
		glm::vec3 entityScale = tc.Scale;

		const reactphysics3d::Vector3 halfExtents(entityScale.x, entityScale.y, entityScale.z);
		reactphysics3d::BoxShape* boxShape = PhysicEngine::GetPhysicsCommon()->createBoxShape(halfExtents);

		if (entity->HasComponent<RigidBodyComponent>())
		{
			auto& rb = entity->GetComponent<RigidBodyComponent>();
			m_Collider = rb.GetRigidBody()->addCollider(boxShape, reactphysics3d::Transform::identity());
		}
	}

	void BoxColliderComponent::UpdateMaterial()
	{
		m_Collider->getMaterial().setMassDensity(mass);
		m_Collider->getMaterial().setBounciness(bounciness);
		m_Collider->getMaterial().setFrictionCoefficient(friction);
	}
}