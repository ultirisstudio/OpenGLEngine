#pragma once

#include <QuasarEngine/Entity/Component.h>

#include <glm/glm.hpp>

namespace reactphysics3d {
	extern "C" {
		class RigidBody;
		class Collider;
		enum class BodyType;
	}
}

namespace QuasarEngine
{
	class RigidBodyComponent : public Component
	{
	private:
		reactphysics3d::RigidBody* rigidbody;
		reactphysics3d::Collider* collider;
	public:
		reactphysics3d::BodyType bodyType;
		std::string bodyTypeString;

		bool enableGravity;

		bool m_LinearAxisFactorX = true;
		bool m_LinearAxisFactorY = true;
		bool m_LinearAxisFactorZ = true;

		bool m_AngularAxisFactorX = true;
		bool m_AngularAxisFactorY = true;
		bool m_AngularAxisFactorZ = true;
	public:
		RigidBodyComponent();
		~RigidBodyComponent();

		void Destroy();

		void Init();
		void Update();

		reactphysics3d::RigidBody* GetRigidBody() { return rigidbody; }

		void SetCollider(reactphysics3d::Collider* collider) { this->collider = collider; }

		void UpdateEnableGravity();
		void UpdateBodyType();
		void UpdateLinearAxisFactor();
		void UpdateAngularAxisFactor();
	};
}