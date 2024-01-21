#pragma once

class Entity;

namespace OpenGLEngine
{
	class MeshComponentPanel
	{
	public:
		MeshComponentPanel() = default;
		~MeshComponentPanel() = default;
		void Render(Entity* entity);
	};
}
