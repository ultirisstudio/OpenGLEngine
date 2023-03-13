#pragma once

#include <OpenGLEngine/Entity/Entity.h>

namespace OpenGLEngine
{
	class EntityPropertiePanel
	{
	public:
		EntityPropertiePanel();

		void OnImGuiRender(Entity* entity);
	private:
		std::shared_ptr<Texture> m_ModelTexture;
	};
}