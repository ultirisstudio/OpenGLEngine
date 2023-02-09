#pragma once

#include <DuckEngine/Resources/Texture.h>

namespace DuckEngine
{
	class Material
	{
	private:
		std::shared_ptr<Texture> m_DiffuseTexture;
		std::shared_ptr<Texture> m_NormalTexture;
		std::shared_ptr<Texture> m_SpecularTexture;

	public:
		Material();
		~Material();

		void ActiveTexture();

		Texture* GetDiffuseTexture();
		void SetDiffuseTexture(const std::string& path);

		Texture* GetNormalTexture();
		void SetNormalTexture(const std::string& path);

		Texture* GetSpecularTexture();
		void SetSpecularTexture(const std::string& path);
	};
}