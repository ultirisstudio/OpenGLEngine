#include "depch.h"
#include "Entity.h"

#include <glad/glad.h>

namespace DuckEngine
{
	void Entity::Draw()
	{
		if (HasComponent<RenderComponent>() && HasComponent<ModelComponent>() && HasComponent<MaterialComponent>())
		{
			if (GetComponent<ModelComponent>().GetPtr())
			{
				Material& material = GetComponent<MaterialComponent>().GetMaterial();
				Model& model = GetComponent<ModelComponent>().GetModel();
				glm::mat4& transform = GetComponent<TransformComponent>().GetTransform();
				Shader& shader = GetComponent<RenderComponent>().GetShader();

				glActiveTexture(GL_TEXTURE0);
				material.GetDiffuseTexture().bind();


				shader.use();

				shader.setUniform("uModel", transform);
				shader.setUniform("uView", Renderer::getViewMatrix());
				shader.setUniform("uProjection", Renderer::getProjectionMatrix());
				shader.setUniform("uTexture", 0);

				model.draw();
			}
		}

		if (HasComponent<SkyboxComponent>())
		{
			auto& sc = GetComponent<SkyboxComponent>();

			glActiveTexture(GL_TEXTURE0);
			sc.m_CubeMap.Bind();
			sc.m_CubeMapShader.use();
			sc.m_CubeMapShader.setUniform("uModel", sc.GetTransform());
			sc.m_CubeMapShader.setUniform("uView", Renderer::getViewMatrix());
			sc.m_CubeMapShader.setUniform("uProjection", Renderer::getProjectionMatrix());
			sc.m_CubeMapShader.setUniform("uCubeMap", 0);
			sc.m_CubeMapShader.setUniform("uView", glm::mat4(glm::mat3(Renderer::getViewMatrix())));
			glDepthFunc(GL_LEQUAL);
			sc.m_Model->draw();
			glDepthFunc(GL_LESS);
		}
	}
}