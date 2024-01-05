#pragma once

#include <OpenGLEngine/Entity/Component.h>

namespace OpenGLEngine
{
	class MaterialComponent : public Component
	{
	private:
		std::shared_ptr<OpenGLEngine::Texture> m_DefaultTexture;
		std::shared_ptr<OpenGLEngine::Texture> m_NoTexture;

		std::shared_ptr<OpenGLEngine::Material> m_Material;
	public:
		std::string m_DiffuseTexture;
		std::string m_SpecularTexture;

		MaterialComponent();

		void InitializeMaterial();

		void addTexture(const std::string& id, const std::string& file);

		OpenGLEngine::Material& GetMaterial() { return *m_Material; }

		OpenGLEngine::Texture& GetDefaultTexture() { return *m_DefaultTexture; }
		OpenGLEngine::Texture& GetNoTexture() { return *m_NoTexture; }

		OpenGLEngine::Texture& GetEditorDiffuseTexture()
		{
			if (m_Material->hasTexture("diffuse"))
			{
				return *m_Material->getTexture("diffuse");
			}

			return *m_NoTexture;
		}

		OpenGLEngine::Texture& GetEditorSpecularTexture()
		{
			if (m_Material->hasTexture("specular"))
			{
				return *m_Material->getTexture("specular");
			}

			return *m_NoTexture;
		}
	};
}