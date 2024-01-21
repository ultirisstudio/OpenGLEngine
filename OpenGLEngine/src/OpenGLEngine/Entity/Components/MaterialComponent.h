#pragma once

#include <OpenGLEngine/Entity/Component.h>

namespace OpenGLEngine
{
	class MaterialComponent : public Component
	{
	private:
		std::shared_ptr<Material> m_Material;
	public:
		std::string m_AlbedoTexture;
		std::string m_NormalTexture;
		std::string m_MetallicTexture;
		std::string m_RoughnessTexture;
		std::string m_AOTexture;

		MaterialComponent();

		void InitializeMaterial();

		void addTexture(const std::string& id, const std::string& file);

		Material& GetMaterial() { return *m_Material; }
	};
}