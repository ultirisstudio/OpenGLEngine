#pragma once

#include <OpenGLEngine/Resources/Texture.h>

class Entity;

namespace OpenGLEngine
{
	class TerrainComponentPanel
	{
	public:
		TerrainComponentPanel();
		~TerrainComponentPanel() = default;

		void Render(Entity* entity);
	private:
		std::shared_ptr<Texture> m_NoTexture;
	};
}
