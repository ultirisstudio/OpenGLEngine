#pragma once

#include <memory>
#include <OpenGLEngine/Resources/Texture.h>

class Entity;

namespace OpenGLEngine
{
	class ModelComponentPanel
	{
	public:
		ModelComponentPanel();
		~ModelComponentPanel() = default;

		void Render(Entity* entity);
	private:
		std::shared_ptr<Texture> m_ModelTexture;
	};
}
