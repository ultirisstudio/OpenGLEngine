#include "qepch.h"
#include "Material.h"

#include <QuasarEngine/Renderer/Renderer.h>

namespace QuasarEngine
{
	Material::Material(const MaterialSpecification& specification) :
		m_Specification(specification)
	{

	}

	Material::~Material()
	{
		
	}

	void Material::SetTexture(TextureType type, std::string path)
	{
		switch (type)
		{
		case TextureType::Albedo:
			m_Specification.AlbedoTexture = path;
			break;
		case TextureType::Normal:
			m_Specification.NormalTexture = path;
			break;
		case TextureType::Metallic:
			m_Specification.MetallicTexture = path;
			break;
		case TextureType::Roughness:
			m_Specification.RoughnessTexture = path;
			break;
		case TextureType::AO:
			m_Specification.AOTexture = path;
			break;
		}
	}

	Texture* Material::GetTexture(TextureType type)
	{
		switch (type)
		{
		case TextureType::Albedo:
			if (HasTexture(type))
				return Renderer::m_SceneData.m_ResourceManager->GetTexture(m_Specification.AlbedoTexture.value()).get();
			else
				return nullptr;
		case TextureType::Normal:
			if (HasTexture(type))
				return Renderer::m_SceneData.m_ResourceManager->GetTexture(m_Specification.NormalTexture.value()).get();
			else
				return nullptr;
		case TextureType::Metallic:
			if (HasTexture(type))
				return Renderer::m_SceneData.m_ResourceManager->GetTexture(m_Specification.MetallicTexture.value()).get();
			else
				return nullptr;
		case TextureType::Roughness:
			if (HasTexture(type))
				return Renderer::m_SceneData.m_ResourceManager->GetTexture(m_Specification.RoughnessTexture.value()).get();
			else
				return nullptr;
		case TextureType::AO:
			if (HasTexture(type))
				return Renderer::m_SceneData.m_ResourceManager->GetTexture(m_Specification.AOTexture.value()).get();
			else
				return nullptr;
		}
		return nullptr;
	}

	bool Material::HasTexture(TextureType type)
	{
		switch (type)
		{
		case TextureType::Albedo:
			return m_Specification.AlbedoTexture.has_value();
		case TextureType::Normal:
			return m_Specification.NormalTexture.has_value();
		case TextureType::Metallic:
			return m_Specification.MetallicTexture.has_value();
		case TextureType::Roughness:
			return m_Specification.RoughnessTexture.has_value();
		case TextureType::AO:
			return m_Specification.AOTexture.has_value();
		}
		return false;
	}

	void Material::ResetTexture(TextureType type)
	{
		switch (type)
		{
		case TextureType::Albedo:
			m_Specification.AlbedoTexture.reset();
			break;
		case TextureType::Normal:
			m_Specification.NormalTexture.reset();
			break;
		case TextureType::Metallic:
			m_Specification.MetallicTexture.reset();
			break;
		case TextureType::Roughness:
			m_Specification.RoughnessTexture.reset();
			break;
		case TextureType::AO:
			m_Specification.AOTexture.reset();
			break;
		}
	}

	std::shared_ptr<Material> Material::CreateMaterial(const MaterialSpecification& specification)
	{
		return std::make_shared<Material>(specification);
	}
}
