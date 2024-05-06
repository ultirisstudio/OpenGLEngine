#pragma once

#include <OpenGLEngine/Entity/Component.h>

namespace reactphysics3d {
	extern "C" {
		class TriangleVertexArray;
	}
}

namespace OpenGLEngine
{
	class MeshColliderComponent : public Component
	{
	private:
		reactphysics3d::TriangleVertexArray* triangleVertexArray;
	public:
		MeshColliderComponent();

		void Init();
	};
}