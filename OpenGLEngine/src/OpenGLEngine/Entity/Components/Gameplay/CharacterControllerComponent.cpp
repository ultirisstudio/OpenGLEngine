#include "depch.h"
#include "CharacterControllerComponent.h"

#include <reactphysics3d/reactphysics3d.h>
#include <reactphysics3d/decimal.h>

#include <glm/glm.hpp>

#include <OpenGLEngine/Entity/Entity.h>
#include <OpenGLEngine/Physic/PhysicEngine.h>
#include <OpenGLEngine/Entity/Components/TransformComponent.h>

namespace OpenGLEngine
{
	CharacterControllerComponent::CharacterControllerComponent() : m_Collider(nullptr), mass(1.0f), friction(0.3f), bounciness(0.3f), m_Height(1.0f), m_Radius(0.5f), m_Rigidbody(nullptr)
	{
	}

	CharacterControllerComponent::~CharacterControllerComponent()
	{
	}

	void CharacterControllerComponent::Init()
	{
		auto& tc = Component::entity->GetComponent<TransformComponent>();
		glm::vec3 entityPosition = tc.Position;
		glm::vec3 entityRotation = tc.Rotation;
		glm::vec3 entityScale = tc.Scale;

		reactphysics3d::Vector3 position(entityPosition.x, entityPosition.y, entityPosition.z);
		reactphysics3d::Vector3 rotation(entityRotation.x, entityRotation.y, entityRotation.z);

		reactphysics3d::Quaternion orientation = reactphysics3d::Quaternion::fromEulerAngles(rotation);

		reactphysics3d::Transform transform(position, orientation);

		m_Rigidbody = PhysicEngine::GetPhysicWorld()->createRigidBody(transform);
		m_Rigidbody->setType(reactphysics3d::BodyType::DYNAMIC);
		m_Rigidbody->enableGravity(true);
		m_Rigidbody->setIsDebugEnabled(true);

		reactphysics3d::CapsuleShape* capsuleShape = PhysicEngine::GetPhysicsCommon()->createCapsuleShape(m_Radius, m_Height);

		m_Collider = m_Rigidbody->addCollider(capsuleShape, reactphysics3d::Transform::identity());

		m_LastPosition = glm::vec3(position.x, position.y, position.z);
		m_LastRotation = glm::vec3(rotation.x, rotation.y, rotation.z);
	}

	void CharacterControllerComponent::Update()
	{
		auto& tc = Component::entity->GetComponent<TransformComponent>();

		const reactphysics3d::Transform& transform = m_Rigidbody->getTransform();

		const reactphysics3d::Vector3& position = transform.getPosition();
		const reactphysics3d::Quaternion& orientation = transform.getOrientation();

		const reactphysics3d::Vector3& rotation = orientation.getVectorV();

		glm::vec3 deltaPosition = glm::vec3(position.x, position.y, position.z) - m_LastPosition;
		glm::vec3 deltaRotation = glm::vec3(rotation.x, rotation.y, rotation.z) - m_LastRotation;

		tc.Position = glm::vec3(position.x, position.y, position.z);
		//tc.Position += deltaPosition;

		//glm::quat rotation = glm::quat(orientation.w, orientation.x, orientation.y, orientation.z);
		//glm::vec3 rotation = glm::eulerAngles(rotationQuat);
		//tc.Rotation = glm::eulerAngles(rotation);
		//tc.Rotation += deltaRotation;
	}

	void CharacterControllerComponent::UpdateColliderMaterial()
	{
		m_Collider->getMaterial().setMassDensity(mass);
		m_Collider->getMaterial().setBounciness(bounciness);
		m_Collider->getMaterial().setFrictionCoefficient(friction);
	}

	void CharacterControllerComponent::UpdateColliderSize()
	{
		m_Rigidbody->removeCollider(m_Collider);

		reactphysics3d::CapsuleShape* capsuleShape = PhysicEngine::GetPhysicsCommon()->createCapsuleShape(m_Radius, m_Height);

		if (capsuleShape)
			m_Collider = m_Rigidbody->addCollider(capsuleShape, reactphysics3d::Transform::identity());
	}

	void CharacterControllerComponent::UpdateLinearAxisFactor()
	{
		m_Rigidbody->setLinearLockAxisFactor(reactphysics3d::Vector3(m_LinearAxisFactorX, m_LinearAxisFactorY, m_LinearAxisFactorZ));
	}

	void CharacterControllerComponent::UpdateAngularAxisFactor()
	{
		m_Rigidbody->setAngularLockAxisFactor(reactphysics3d::Vector3(m_AngularAxisFactorX, m_AngularAxisFactorY, m_AngularAxisFactorZ));
	}

	void CharacterControllerComponent::Move(glm::vec3 direction)
	{
		//->applyLocalForceAtCenterOfMass(reactphysics3d::Vector3(force.x, force.y, force.z))
		reactphysics3d::Vector3 moveDirection(direction.x, direction.y, direction.z);
		m_Rigidbody->setLinearVelocity(moveDirection);
	}
}