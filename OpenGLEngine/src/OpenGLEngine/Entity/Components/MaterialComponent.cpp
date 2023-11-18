#include "depch.h"
#include "MaterialComponent.h"

MaterialComponent::MaterialComponent()
{
	m_Material = OpenGLEngine::Material::CreateMaterial();
	m_DefaultTexture = OpenGLEngine::Texture::CreateTexture("Assets/Textures/white_texture.jpg");
	m_NoTexture = OpenGLEngine::Texture::CreateTexture("Assets/Textures/3d-modeling.png");
}

void MaterialComponent::InitializeMaterial()
{
	m_Material->addVec3("ambient", glm::vec3(0.1f));
	m_Material->addVec3("diffuse", glm::vec3(1.0f));
	m_Material->addVec3("specular", glm::vec3(1.0f));
	m_Material->addBoolean("diffuse", false);
	m_Material->addBoolean("specular", false);
	m_Material->addFloat("shininess", 32.0f);
}

void MaterialComponent::addTexture(const std::string& id, const std::string& file)
{
	m_Material->addTexture(id, file);
	m_Material->addBoolean(id, true);

	if (id == "diffuse")
	{
		m_DiffuseTexture = file;
	}
	else if (id == "specular")
	{
		m_SpecularTexture = file;
	}
}
