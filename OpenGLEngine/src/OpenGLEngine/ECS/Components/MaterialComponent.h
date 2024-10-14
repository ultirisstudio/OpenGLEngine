#pragma once

#include <OpenGLEngine/ECS/Component.h>
#include <OpenGLEngine/Resources/Materials/Material.h>

namespace OpenGLEngine
{
	class MaterialComponent : public Component
	{
	private:
		std::shared_ptr<Material> m_Material;
	public:
		MaterialComponent();
		MaterialComponent(const MaterialSpecification& specification);

		Material& GetMaterial();
	};
}