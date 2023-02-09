#include "depch.h"
#include <DuckEngine/Resources/Material.h>
#include <DuckEngine/Renderer/Renderer.h>
#include <glad/glad.h>

namespace DuckEngine
{
	Material::Material()
	{
		
	}

	Material::~Material()
	{

	}

	void Material::ActiveTexture()
	{
		glActiveTexture(GL_TEXTURE0);
	}

	Texture* Material::GetDiffuseTexture()
	{
		return m_DiffuseTexture.get();
	}

	void Material::SetDiffuseTexture(const std::string& path)
	{
		m_DiffuseTexture = Renderer::CreateTexture(path);
	}

	Texture* Material::GetNormalTexture()
	{
		return m_NormalTexture.get();
	}

	void Material::SetNormalTexture(const std::string& path)
	{
		m_NormalTexture = Renderer::CreateTexture(path);
	}

	Texture* Material::GetSpecularTexture()
	{
		return m_SpecularTexture.get();
	}

	void Material::SetSpecularTexture(const std::string& path)
	{
		m_SpecularTexture = Renderer::CreateTexture(path);
	}
}