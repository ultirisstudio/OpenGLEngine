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
		float mass;
		float friction;
		float bounciness;
	public:
		RigidBodyComponent();

		void Init();
		void Update();

		void UpdateEnableGravity();
		void UpdateBodyType();
		void UpdateMaterial();
	};
}