#include "depch.h"
#include "Component.h"

RenderComponent::RenderComponent() : m_Shader()
{
	m_Shader.LoadFromFile("Shaders/texture.vert", "Shaders/texture.frag");
}

void RenderComponent::Draw()
{
	if (entity->HasComponent<ModelComponent>() && entity->HasComponent<MaterialComponent>())
	{
		if (entity->GetComponent<ModelComponent>().GetPtr())
		{
			DuckEngine::Material& material = entity->GetComponent<MaterialComponent>().GetMaterial();
			DuckEngine::Model& model = entity->GetComponent<ModelComponent>().GetModel();
			glm::mat4& transform = entity->GetComponent<TransformComponent>().GetTransform();
			DuckEngine::Shader& shader = entity->GetComponent<RenderComponent>().GetShader();

			material.ActiveTexture();
			material.GetDiffuseTexture().bind();

			shader.use();

			shader.setUniform("uModel", transform);
			shader.setUniform("uView", DuckEngine::Renderer::getViewMatrix());
			shader.setUniform("uProjection", DuckEngine::Renderer::getProjectionMatrix());
			shader.setUniform("uTexture", 0);

			model.draw();
		}
	}

	if (entity->HasComponent<SkyboxComponent>())
	{
		auto& sc = entity->GetComponent<SkyboxComponent>();

		sc.m_CubeMap.ActiveTexture();
		sc.m_CubeMap.Bind();
		sc.m_CubeMapShader.use();
		sc.m_CubeMapShader.setUniform("uModel", sc.GetTransform());
		sc.m_CubeMapShader.setUniform("uView", DuckEngine::Renderer::getViewMatrix());
		sc.m_CubeMapShader.setUniform("uProjection", DuckEngine::Renderer::getProjectionMatrix());
		sc.m_CubeMapShader.setUniform("uCubeMap", 0);
		sc.m_CubeMapShader.setUniform("uView", glm::mat4(glm::mat3(DuckEngine::Renderer::getViewMatrix())));
		sc.m_CubeMap.BeginDrawModel();
		sc.m_Model->draw();
		sc.m_CubeMap.EndDrawModel();
	}
}
