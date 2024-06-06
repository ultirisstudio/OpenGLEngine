#include "depch.h"
#include "MaterialComponent.h"

namespace OpenGLEngine
{
	MaterialComponent::MaterialComponent()
	{
		MaterialSpecification specification;
		m_Material = Material::CreateMaterial(specification);
	}

	MaterialComponent::MaterialComponent(const MaterialSpecification& specification)
	{
		m_Material = Material::CreateMaterial(specification);
	}

	Material& MaterialComponent::GetMaterial()
	{
		return *m_Material;
	}
}