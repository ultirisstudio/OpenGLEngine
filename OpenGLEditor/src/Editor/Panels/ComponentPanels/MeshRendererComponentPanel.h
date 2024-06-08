#pragma once

class Entity;

namespace OpenGLEngine
{
	class MeshRendererComponentPanel
	{
	public:
		MeshRendererComponentPanel() = default;
		~MeshRendererComponentPanel() = default;
		void Render(Entity* entity);
	};
}
