#pragma once

#include <QuasarEngine/Entity/Component.h>

#include <glm/glm.hpp>

namespace reactphysics3d {
	extern "C" {
		class RigidBody;
		class Collider;
	}
}

namespace QuasarEngine
{
	class CharacterControllerComponent : public Component
	{
	private:
		reactphysics3d::RigidBody* m_Rigidbody;
		reactphysics3d::Collider* m_Collider;

		glm::vec3 m_LastPosition;
		glm::vec3 m_LastRotation;
	public:
		float mass;
		float friction;
		float bounciness;
		
		float m_Radius;
		float m_Height;

		bool m_LinearAxisFactorX = true;
		bool m_LinearAxisFactorY = true;
		bool m_LinearAxisFactorZ = true;

		bool m_AngularAxisFactorX = true;
		bool m_AngularAxisFactorY = true;
		bool m_AngularAxisFactorZ = true;
	public:
		CharacterControllerComponent();
		~CharacterControllerComponent();

		reactphysics3d::RigidBody* GetRigidBody() { return m_Rigidbody; }

		void Init();
		void Update();
		void UpdateColliderMaterial();
		void UpdateColliderSize();
		void UpdateLinearAxisFactor();
		void UpdateAngularAxisFactor();

		void Move(glm::vec3 direction);
	};
}