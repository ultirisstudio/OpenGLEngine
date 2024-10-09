#pragma once

#include <memory>
#include <OpenGLEngine/Resources/Texture.h>

namespace OpenGLEngine
{
	class Entity;

	class MaterialComponentPanel
	{
	public:
		MaterialComponentPanel();
		~MaterialComponentPanel() = default;

		void Render(Entity entity);
	private:
		std::shared_ptr<Texture> m_NoTexture;
	};
}
