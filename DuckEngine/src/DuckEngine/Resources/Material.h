#pragma once

#include <DuckEngine/Resources/Texture.h>

namespace DuckEngine
{
	class Material
	{
	private:
		std::shared_ptr<Texture> m_DiffuseTexture;

	public:
		Material(const std::string& path);
		~Material();

		Texture& GetDiffuseTexture();
		void SetDiffuseTexture(const std::string& path);
	};
}