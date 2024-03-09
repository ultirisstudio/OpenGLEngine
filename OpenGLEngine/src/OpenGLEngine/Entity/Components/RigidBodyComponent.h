#pragma once

#include <OpenGLEngine/Entity/Component.h>

#include <glm/glm.hpp>

namespace reactphysics3d {
	extern "C" {
		class RigidBody;
	}
}

namespace OpenGLEngine
{
	class RigidBodyComponent : public Component
	{
	private:
		glm::vec3 lastPosition;
		glm::vec3 lastRotation;
	public:
		reactphysics3d::RigidBody* rigidbody;

		RigidBodyComponent();

		void Init();
		void Update();
	};
}