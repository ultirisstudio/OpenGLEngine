#pragma once

#include <OpenGLEngine/Entity/Component.h>

namespace reactphysics3d {
	extern "C" {
		class RigidBody;
	}
}

namespace OpenGLEngine
{
	class RigidBodyComponent : public Component
	{
	public:
		reactphysics3d::RigidBody* rigidbody;

		RigidBodyComponent();

		void Init();
	};
}