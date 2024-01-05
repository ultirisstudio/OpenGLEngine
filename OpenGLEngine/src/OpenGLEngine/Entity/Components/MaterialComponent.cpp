#include "depch.h"
#include "MaterialComponent.h"

namespace OpenGLEngine
{
	MaterialComponent::MaterialComponent()
	{
		m_Material = Material::CreateMaterial();
		m_DefaultTexture = Texture::CreateTexture("Assets/Textures/white_texture.jpg", false);
		m_NoTexture = Texture::CreateTexture("Assets/Textures/3d-modeling.png", false);
	}

	void MaterialComponent::InitializeMaterial()
	{
		m_Material->addVec3("ambient", glm::vec3(0.1f));
		m_Material->addVec3("diffuse", glm::vec3(1.0f));
		m_Material->addVec3("specular", glm::vec3(0.0f));
		m_Material->addBoolean("diffuse", false);
		m_Material->addBoolean("specular", false);
		m_Material->addFloat("shininess", 32.0f);
	}

	void MaterialComponent::addTexture(const std::string& id, const std::string& file)
	{
		if (id == "diffuse")
		{
			m_DiffuseTexture = file;
			m_Material->addTexture(id, file, true);
		}
		else if (id == "specular")
		{
			m_SpecularTexture = file;
			m_Material->addTexture(id, file, false);
		}

		m_Material->addBoolean(id, true);
	}
}