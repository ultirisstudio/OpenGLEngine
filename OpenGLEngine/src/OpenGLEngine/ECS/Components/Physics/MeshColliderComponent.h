#pragma once

#include <OpenGLEngine/ECS/Component.h>

namespace reactphysics3d {
	extern "C" {
		class ConcaveMeshShape;
		class ConvexMeshShape;
	}
}

namespace OpenGLEngine
{
	class MeshColliderComponent : public Component
	{
	private:
		reactphysics3d::ConcaveMeshShape* m_concaveMeshShape;
		reactphysics3d::ConvexMeshShape* m_convexMeshShape;

		void GenerateConcaveMesh();
		void GenerateConvexMesh();
	public:
		MeshColliderComponent();

		void Generate();

		bool m_IsConvex = false;
	};
}