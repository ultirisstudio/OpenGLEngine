#pragma once

#include <QuasarEngine/Entity/Component.h>

namespace reactphysics3d {
	extern "C" {
		class Collider;
	}
}

namespace QuasarEngine
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