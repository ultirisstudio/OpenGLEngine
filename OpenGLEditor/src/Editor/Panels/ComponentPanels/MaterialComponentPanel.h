#pragma once

#include <OpenGLEngine/Resources/Texture.h>

class Entity;

namespace OpenGLEngine
{
	class MaterialComponentPanel
	{
	public:
		MaterialComponentPanel();
		~MaterialComponentPanel() = default;

		void Render(Entity* entity);
	private:
		Texture& m_NoTexture;
	};
}
