#pragma once

class Entity;

namespace OpenGLEngine
{
	class MeshColliderComponentPanel
	{
	public:
		MeshColliderComponentPanel() = default;
		~MeshColliderComponentPanel() = default;

		void Render(Entity* entity);
	};
}