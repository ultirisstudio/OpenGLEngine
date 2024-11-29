#pragma once

#include <QuasarEngine/Resources/Texture2D.h>

namespace QuasarEngine
{
	class Entity;

	class TerrainComponentPanel
	{
	public:
		TerrainComponentPanel();
		~TerrainComponentPanel() = default;

		void Render(Entity entity);
	private:
		std::shared_ptr<Texture2D> m_NoTexture;
	};
}
