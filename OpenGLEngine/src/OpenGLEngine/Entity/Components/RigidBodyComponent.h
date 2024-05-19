#pragma once

#include <OpenGLEngine/Entity/Component.h>

#include <glm/glm.hpp>

namespace reactphysics3d {
	extern "C" {
		class RigidBody;
		class Collider;
		enum class BodyType;
	}
}

namespace OpenGLEngine
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
	public:
		RigidBodyComponent();
		~RigidBodyComponent();

		void Init();
		void Update();

		reactphysics3d::RigidBody* GetRigidBody() { return rigidbody; }

		void SetCollider(reactphysics3d::Collider* collider) { this->collider = collider; }

		void UpdateEnableGravity();
		void UpdateBodyType();
	};
}