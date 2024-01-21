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
		Texture& m_NoTexture;
	};
}
