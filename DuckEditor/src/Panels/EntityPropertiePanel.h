#pragma once

#include <DuckEngine/Entity/Entity.h>

namespace DuckEngine
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