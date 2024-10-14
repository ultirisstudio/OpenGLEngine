#pragma once

#include <memory>
#include <QuasarEngine/Resources/Texture.h>

namespace QuasarEngine
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
