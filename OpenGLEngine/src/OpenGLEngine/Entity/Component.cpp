#include "depch.h"
#include <OpenGLEngine/Entity/Component.h>
#include <OpenGLEngine/Shader/Generators/ShaderGenerator.h>
#include <glad/glad.h>

RenderComponent::RenderComponent() : m_Shader(), m_CanDraw(true)
{
	
}

void RenderComponent::Draw()
{
	if (!m_CanDraw)
		return;

	if (entity->HasComponent<ModelComponent>() && entity->HasComponent<MaterialComponent>() && entity->GetComponent<ModelComponent>().GetPtr())
	{
		OpenGLEngine::Material& material = entity->GetComponent<MaterialComponent>().GetMaterial();
		OpenGLEngine::Model& model = entity->GetComponent<ModelComponent>().GetModel();
		glm::mat4& transform = entity->GetComponent<TransformComponent>().GetTransform();
		OpenGLEngine::Shader& shader = entity->GetComponent<RenderComponent>().GetShader();

		int nat = 0;

		shader.use();

		shader.setUniform("uModel", transform);
		shader.setUniform("uView", OpenGLEngine::Renderer::m_SceneData.m_ActiveCamera->getViewMatrix());
		shader.setUniform("uProjection", OpenGLEngine::Renderer::m_SceneData.m_ActiveCamera->getProjectionMatrix());

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

	if (entity->HasComponent<SkyboxComponent>())
	{
		auto& sc = entity->GetComponent<SkyboxComponent>();

		sc.GetCubeMap()->ActiveTexture();
		sc.GetCubeMap()->Bind();
		sc.GetCubeMapShader()->use();
		sc.GetCubeMapShader()->setUniform("uView", glm::mat4(glm::mat3(OpenGLEngine::Renderer::m_SceneData.m_ActiveCamera->getViewMatrix())));
		sc.GetCubeMapShader()->setUniform("uProjection", OpenGLEngine::Renderer::m_SceneData.m_ActiveCamera->getProjectionMatrix());
		sc.GetCubeMapShader()->setUniform("uCubeMap", 0);
		sc.GetCubeMap()->BeginDrawModel();
		sc.GetModel()->draw();
		sc.GetCubeMap()->EndDrawModel();
	}
}

void RenderComponent::GenerateShader()
{
	if (entity->HasComponent<MaterialComponent>())
	{
		OpenGLEngine::ShaderGenerator shaderGenerator(entity->GetComponent<MaterialComponent>().GetMaterial(), OpenGLEngine::ShaderType::BPhong);
		const std::string& vs = shaderGenerator.generateVertexShader();
		const std::string& fs = shaderGenerator.generateFragmentShader();
		m_Shader.LoadFromSource(vs, fs, shaderGenerator.getVertexShaderRenderInfo(), shaderGenerator.getFragmentShaderRenderInfo());
	}
}

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
