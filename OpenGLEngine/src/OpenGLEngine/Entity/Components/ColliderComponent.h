#pragma once

#include <OpenGLEngine/Entity/Component.h>

#include <glm/glm.hpp>

namespace reactphysics3d {
	extern "C" {
		class RigidBody;
		class CollisionBody;
		class Collider;
	}
}

namespace OpenGLEngine
{
	class ColliderComponent : public Component
	{
	private:
		glm::vec3 lastPosition;
		glm::vec3 lastRotation;
		glm::vec3 lastScale;
	public:
		reactphysics3d::RigidBody* rigidbody;
		reactphysics3d::CollisionBody* collisionBody;
		reactphysics3d::Collider* collider;

		ColliderComponent();

		void Init();
		void Update();
	};
}