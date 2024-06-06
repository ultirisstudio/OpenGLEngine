#pragma once

#include <string>
#include <optional>
#include <glm/glm.hpp>

#include <OpenGLEngine/Resources/Texture.h>

namespace OpenGLEngine
{
	enum TextureType
	{
		Albedo,
		Normal,
		Metallic,
		Roughness,
		AO
	};

	struct MaterialSpecification
	{
		std::optional<std::string> AlbedoTexture;
		std::optional<std::string> NormalTexture;
		std::optional<std::string> MetallicTexture;
		std::optional<std::string> RoughnessTexture;
		std::optional<std::string> AOTexture;

		glm::vec3 Albedo = glm::vec3(1.0f);

		float Metallic = 0.5f;
		float Roughness = 0.5f;
		float AO = 1.0f;
	};

	class Material
	{
	private:
		MaterialSpecification m_Specification;

	public:
		Material(const MaterialSpecification& specification);
		~Material();

		void SetTexture(TextureType type, std::string path);
		Texture* GetTexture(TextureType type);

		bool HasTexture(TextureType type);

		const MaterialSpecification& GetSpecification() { return m_Specification; }

		glm::vec3& GetAlbedo() { return m_Specification.Albedo; }

		float& GetMetallic() { return m_Specification.Metallic; }
		float& GetRoughness() { return m_Specification.Roughness; }
		float& GetAO() { return m_Specification.AO; }

		void ResetTexture(TextureType type);

		static std::shared_ptr<Material> CreateMaterial(const MaterialSpecification& specification);
	};
}
