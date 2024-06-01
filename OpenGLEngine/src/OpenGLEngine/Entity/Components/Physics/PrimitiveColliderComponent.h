#pragma once

#include <OpenGLEngine/Entity/Component.h>

namespace reactphysics3d {
	extern "C" {
		class Collider;
	}
}

namespace OpenGLEngine
{
	class PrimitiveColliderComponent : public Component
	{
	protected:
		reactphysics3d::Collider* m_Collider;
	public:
		float mass;
		float friction;
		float bounciness;

		PrimitiveColliderComponent();

		virtual void Init() = 0;
		virtual void UpdateColliderMaterial() = 0;
		virtual void UpdateColliderSize() = 0;
	};
}