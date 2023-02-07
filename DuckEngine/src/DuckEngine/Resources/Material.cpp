#include "depch.h"
#include <DuckEngine/Resources/Material.h>
#include <DuckEngine/Renderer/Renderer.h>
#include <glad/glad.h>

namespace DuckEngine
{
	Material::Material(const std::string& path)
	{
		m_DiffuseTexture = Renderer::CreateTexture(path);
	}

	Material::~Material()
	{

	}

	void Material::ActiveTexture()
	{
		glActiveTexture(GL_TEXTURE0);
	}

	Texture& Material::GetDiffuseTexture()
	{
		return *m_DiffuseTexture;
	}

	void Material::SetDiffuseTexture(const std::string& path)
	{
		m_DiffuseTexture = Renderer::CreateTexture(path);
	}
}