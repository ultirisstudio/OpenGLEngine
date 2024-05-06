#pragma once

#include <OpenGLEngine/Entity/Component.h>

#include <glm/glm.hpp>

namespace reactphysics3d {
	extern "C" {
		class RigidBody;
		class Collider;
	}
}

namespace OpenGLEngine
{
	class BoxColliderComponent : public Component
	{
	public:
		reactphysics3d::RigidBody* rigidbody;
		reactphysics3d::Collider* collider;

		BoxColliderComponent();
		~BoxColliderComponent();

		void Init();
	};
}