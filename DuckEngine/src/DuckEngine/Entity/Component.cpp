#include "depch.h"
#include <DuckEngine/Entity/Component.h>
#include <DuckEngine/Shader/Generators/ShaderGenerator.h>
#include <glad/glad.h>

RenderComponent::RenderComponent() : m_Shader()
{
	
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

			int nat = 0;

			shader.use();

			shader.setUniform("uModel", transform);
			shader.setUniform("uView", DuckEngine::Renderer::getViewMatrix());
			shader.setUniform("uProjection", DuckEngine::Renderer::getProjectionMatrix());

			for (auto& i : shader.GetFragmentRenderInfo().getRenderInfo())
			{
				if (i.first == "uMaterial.ambient")
				{
					if (i.second == "vec3")
					{
						shader.setUniform(i.first, *material.getVec3("ambient"));
					}
					else if (i.second == "sampler2D")
					{
						glActiveTexture(GL_TEXTURE0 + nat);
						material.getTexture("ambient")->bind();
						shader.setUniform(i.first, nat);
						nat++;
					}
				}
				else if (i.first == "uMaterial.diffuse")
				{
					if (i.second == "vec3")
					{
						shader.setUniform(i.first, *material.getVec3("diffuse"));
					}
					else if (i.second == "sampler2D")
					{
						glActiveTexture(GL_TEXTURE0 + nat);
						material.getTexture("diffuse")->bind();
						shader.setUniform(i.first, nat);
						nat++;
					}
				}
				else if (i.first == "uMaterial.specular")
				{
					if (i.second == "vec3")
					{
						shader.setUniform(i.first, *material.getVec3("specular"));
					}
					else if (i.second == "sampler2D")
					{
						glActiveTexture(GL_TEXTURE0 + nat);
						material.getTexture("specular")->bind();
						shader.setUniform(i.first, nat);
						nat++;
					}
				}
				else if (i.first == "uMaterial.shininess")
				{
					shader.setUniform(i.first, *material.getFloat("shininess"));
				}
			}

			shader.setUniform("uPointLight.ambient", glm::vec3(0.2f));
			shader.setUniform("uPointLight.diffuse", glm::vec3(0.8f));
			shader.setUniform("uPointLight.specular", glm::vec3(1.0f));
			shader.setUniform("uPointLight.position", glm::vec3({0.0f, 1.5f, 2.0f}));
			shader.setUniform("uPointLight.constant", 1.0f);
			shader.setUniform("uPointLight.linear", 0.2f);
			shader.setUniform("uPointLight.quadratic", 0.08f);

			shader.setUniform("uViewPos", glm::vec3(2.0f));

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

void RenderComponent::GenerateShader()
{
	DuckEngine::ShaderGenerator shaderGenerator(entity->GetComponent<MaterialComponent>().GetMaterial(), DuckEngine::ShaderType::BPhong);
	const std::string& vs = shaderGenerator.generateVertexShader();
	const std::string& fs = shaderGenerator.generateFragmentShader();

	//std::cout << vs << std::endl;
	//std::cout << fs << std::endl;

	m_Shader.LoadFromSource(vs, fs, shaderGenerator.getVertexShaderRenderInfo(), shaderGenerator.getFragmentShaderRenderInfo());
}
