#pragma once

namespace OpenGLEngine
{
	class Entity;

	class MeshColliderComponentPanel
	{
	public:
		MeshColliderComponentPanel() = default;
		~MeshColliderComponentPanel() = default;

		void Render(Entity entity);
	};
}