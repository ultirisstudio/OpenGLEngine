#include "depch.h"
#include "MaterialComponent.h"

namespace OpenGLEngine
{
	MaterialComponent::MaterialComponent()
	{
		m_Material = Material::CreateMaterial();
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
}