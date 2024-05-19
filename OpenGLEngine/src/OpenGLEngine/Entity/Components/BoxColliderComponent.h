#pragma once

#include <OpenGLEngine/Entity/Component.h>

#include <glm/glm.hpp>

namespace reactphysics3d {
	extern "C" {
		class Collider;
	}
}

namespace OpenGLEngine
{
	class BoxColliderComponent : public Component
	{
	private:
		reactphysics3d::Collider* m_Collider;
	public:
		float mass;
		float friction;
		float bounciness;

		BoxColliderComponent();
		~BoxColliderComponent();

		void Init();
		void UpdateMaterial();
	};
}