#include "depch.h"
#include "MaterialComponent.h"

namespace OpenGLEngine
{
	MaterialComponent::MaterialComponent()
	{
		m_Material = Material::CreateMaterial();
		m_NoTexture = Texture::CreateTexture("Assets/Textures/3d-modeling.png", false);
	}

	void MaterialComponent::InitializeMaterial()
	{
		m_Material->addVec3("albedo", glm::vec3(1.0f));
		m_Material->addFloat("metallic", 0.5f);
		m_Material->addFloat("roughness", 0.5f);
		m_Material->addFloat("ao", 1.0f);

		m_Material->addBoolean("albedo", false);
		m_Material->addBoolean("normal", false);
		m_Material->addBoolean("metallic", false);
		m_Material->addBoolean("roughness", false);
		m_Material->addBoolean("ao", false);
	}

	void MaterialComponent::addTexture(const std::string& id, const std::string& file)
	{
		if (id == "albedo")
		{
			m_AlbedoTexture = file;
			m_Material->addTexture(id, file, true);
			m_Material->addBoolean(id, true);
		}
		else if (id == "normal")
		{
			m_NormalTexture = file;
			m_Material->addTexture(id, file, false);
			m_Material->addBoolean(id, true);
		}
		else if (id == "metallic")
		{
			m_MetallicTexture = file;
			m_Material->addTexture(id, file, false);
			m_Material->addBoolean(id, true);
		}
		else if (id == "roughness")
		{
			m_RoughnessTexture = file;
			m_Material->addTexture(id, file, false);
			m_Material->addBoolean(id, true);
		}
		else if (id == "ao")
		{
			m_AOTexture = file;
			m_Material->addTexture(id, file, false);
			m_Material->addBoolean(id, true);
		}
		else
		{
			std::cout << "Error: MaterialComponent::addTexture() - Invalid texture id: " << id << std::endl;
		}
	}

	Texture& MaterialComponent::GetEditorAlbedoTexture()
	{
		if (m_Material->hasTexture("albedo"))
		{
			return *m_Material->getTexture("albedo");
		}
		return *m_NoTexture;
	}

	Texture& MaterialComponent::GetEditorNormalTexture()
	{
		if (m_Material->hasTexture("normal"))
		{
			return *m_Material->getTexture("normal");
		}
		return *m_NoTexture;
	}

	Texture& MaterialComponent::GetEditorMetallicTexture()
	{
		if (m_Material->hasTexture("metallic"))
		{
			return *m_Material->getTexture("metallic");
		}
		return *m_NoTexture;
	}

	Texture& MaterialComponent::GetEditorRoughnessTexture()
	{
		if (m_Material->hasTexture("roughness"))
		{
			return *m_Material->getTexture("roughness");
		}
		return *m_NoTexture;
	}

	Texture& MaterialComponent::GetEditorAOTexture()
	{
		if (m_Material->hasTexture("ao"))
		{
			return *m_Material->getTexture("ao");
		}
		return *m_NoTexture;
	}
}