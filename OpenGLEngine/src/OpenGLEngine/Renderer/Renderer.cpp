#include "depch.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>

#include <OpenGLEngine/Core/Application.h>
#include <OpenGLEngine/Renderer/Renderer.h>
#include <OpenGLEngine/Shader/Generators/ShaderGenerator.h>
#include <OpenGLEngine/Tools/Log.h>

#include <OpenGLEngine/Scene/Skybox.h>

#include <OpenGLEngine/Entity/Components/TransformComponent.h>
#include <OpenGLEngine/Entity/Components/ModelComponent.h>
#include <OpenGLEngine/Entity/Components/MeshComponent.h>
#include <OpenGLEngine/Entity/Components/MaterialComponent.h>
#include <OpenGLEngine/Entity/Components/LightComponent.h>
#include <OpenGLEngine/Entity/Components/TerrainComponent.h>

#include <OpenGLEngine/Tools/Math.h>

namespace OpenGLEngine {
	Renderer::SceneData Renderer::m_SceneData = Renderer::SceneData();

	void Renderer::Init()
	{
		m_SceneData.m_Shader = Shader();
		m_SceneData.m_Shader.LoadFromFile("Shaders/pbr_shader.vert", "Shaders/pbr_shader.frag");
	}

	void Renderer::BeginScene(Scene& scene)
	{
		m_SceneData.m_Scene = &scene;
	}

	void Renderer::Render(bool runtime)
	{
		m_SceneData.m_Shader.use();

		glm::mat4 viewMatrix;
		glm::mat4 projectionMatrix;
		if (runtime)
		{
			viewMatrix = m_SceneData.m_Scene->getActiveCamera()->getViewMatrix();
			projectionMatrix = m_SceneData.m_Scene->getActiveCamera()->getProjectionMatrix();

			glm::vec3 position, rotation, scale;
			Math::DecomposeTransform(m_SceneData.m_Scene->getActiveCamera()->GetTransform(), position, rotation, scale);

			m_SceneData.m_Shader.setUniform("uCameraPosition", position);
		}
		else
		{
			viewMatrix = m_SceneData.m_Scene->getEditorCamera().getViewMatrix();
			projectionMatrix = m_SceneData.m_Scene->getEditorCamera().getProjectionMatrix();

			glm::vec3 position, rotation, scale;
			Math::DecomposeTransform(m_SceneData.m_Scene->getEditorCamera().GetTransform(), position, rotation, scale);

			m_SceneData.m_Shader.setUniform("uCameraPosition", position);
		}

		int dirLightCount = 0;
		int pointLightCount = 0;

		for (auto entity = m_SceneData.m_Scene->getEntities()->begin(); entity != m_SceneData.m_Scene->getEntities()->end(); entity++)
		{
			m_SceneData.m_Shader.use();

			int nat = 0;
			glm::mat4& transform = entity->second.GetComponent<TransformComponent>().GetTransform();

			if (entity->second.HasComponent<LightComponent>())
			{
				auto& lc = entity->second.GetComponent<LightComponent>();
				if (lc.lightType == LightComponent::LightType::DIRECTIONAL)
				{
					//m_SceneData.m_Shader.setUniform("dirLights[" + std::to_string(dirLightCount) + "].direction", entity->second.GetComponent<TransformComponent>().Rotation);
					//m_SceneData.m_Shader.setUniform("dirLights[" + std::to_string(dirLightCount) + "].diffuse", lc.dir_diffuse);
					//m_SceneData.m_Shader.setUniform("dirLights[" + std::to_string(dirLightCount) + "].specular", lc.dir_specular);

					dirLightCount++;
				}
				else if (lc.lightType == LightComponent::LightType::POINT)
				{
					m_SceneData.m_Shader.setUniform("uPointLights[" + std::to_string(pointLightCount) + "].position", entity->second.GetComponent<TransformComponent>().Position);
					m_SceneData.m_Shader.setUniform("uPointLights[" + std::to_string(pointLightCount) + "].color", lc.point_color);
					m_SceneData.m_Shader.setUniform("uPointLights[" + std::to_string(pointLightCount) + "].constant", lc.point_constant);
					m_SceneData.m_Shader.setUniform("uPointLights[" + std::to_string(pointLightCount) + "].linear", lc.point_linear);
					m_SceneData.m_Shader.setUniform("uPointLights[" + std::to_string(pointLightCount) + "].quadratic", lc.point_quadratic);

					pointLightCount++;
				}
			}

			if (entity->second.HasComponent<ModelComponent>() && entity->second.GetComponent<ModelComponent>().GetPtr())
			{
				m_SceneData.m_Shader.use();

				for (Entity subEntity : *entity->second.GetComponent<ModelComponent>().GetSubEntities())
				{
					m_SceneData.m_Shader.use();

					Material& material = subEntity.GetComponent<MaterialComponent>().GetMaterial();

					m_SceneData.m_Shader.setUniform("uModel", transform);
					m_SceneData.m_Shader.setUniform("uView", viewMatrix);
					m_SceneData.m_Shader.setUniform("uProjection", projectionMatrix);
					m_SceneData.m_Shader.setUniform("uNormalMatrix", glm::transpose(glm::inverse(glm::mat3(transform))));

					m_SceneData.m_Shader.setUniform("uMaterial.albedoColor", *material.getVec3("albedo"));
					m_SceneData.m_Shader.setUniform("uMaterial.metallic", *material.getFloat("metallic"));
					m_SceneData.m_Shader.setUniform("uMaterial.roughness", *material.getFloat("roughness"));
					m_SceneData.m_Shader.setUniform("uMaterial.ao", *material.getFloat("ao"));

					m_SceneData.m_Shader.setUniform("uMaterial.use_albedo_texture", static_cast<bool>(*material.getBoolean("albedo")));
					m_SceneData.m_Shader.setUniform("uMaterial.use_normal_texture", static_cast<bool>(*material.getBoolean("normal")));
					m_SceneData.m_Shader.setUniform("uMaterial.use_metallic_texture", static_cast<bool>(*material.getBoolean("metallic")));
					m_SceneData.m_Shader.setUniform("uMaterial.use_roughness_texture", static_cast<bool>(*material.getBoolean("roughness")));
					m_SceneData.m_Shader.setUniform("uMaterial.use_ao_texture", static_cast<bool>(*material.getBoolean("ao")));

					if (*material.getBoolean("albedo"))
					{
						glActiveTexture(GL_TEXTURE0 + nat);
						material.getTexture("albedo")->bind();
						m_SceneData.m_Shader.setUniform("uMaterial.albedoMap", nat);
						nat++;
					}

					if (*material.getBoolean("normal"))
					{
						glActiveTexture(GL_TEXTURE0 + nat);
						material.getTexture("normal")->bind();
						m_SceneData.m_Shader.setUniform("uMaterial.normalMap", nat);
						nat++;
					}

					if (*material.getBoolean("metallic"))
					{
						glActiveTexture(GL_TEXTURE0 + nat);
						material.getTexture("metallic")->bind();
						m_SceneData.m_Shader.setUniform("uMaterial.metallicMap", nat);
						nat++;
					}

					if (*material.getBoolean("roughness"))
					{
						glActiveTexture(GL_TEXTURE0 + nat);
						material.getTexture("roughness")->bind();
						m_SceneData.m_Shader.setUniform("uMaterial.roughnessMap", nat);
						nat++;
					}

					if (*material.getBoolean("ao"))
					{
						glActiveTexture(GL_TEXTURE0 + nat);
						material.getTexture("ao")->bind();
						m_SceneData.m_Shader.setUniform("uMaterial.aoMap", nat);
						nat++;
					}

					glActiveTexture(GL_TEXTURE0 + nat);
					m_SceneData.m_Scene->getSkybox().BindIrradianceMap();
					m_SceneData.m_Shader.setUniform("uIrradianceMap", nat);
					nat++;

					subEntity.GetComponent<MeshComponent>().GetMesh().draw();
				}
				
				if (entity->second.HasComponent<ModelComponent>())
					entity->second.GetComponent<ModelComponent>().GetModel().draw();
			}

			if (entity->second.HasComponent<TerrainComponent>() && entity->second.GetComponent<TerrainComponent>().IsGenerated())
			{
				if (entity->second.GetComponent<TerrainComponent>().m_PolygonMode)
					glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

				entity->second.GetComponent<TerrainComponent>().GetShader().use();

				entity->second.GetComponent<TerrainComponent>().GetShader().setUniform("projection", projectionMatrix);
				entity->second.GetComponent<TerrainComponent>().GetShader().setUniform("view", viewMatrix);
				entity->second.GetComponent<TerrainComponent>().GetShader().setUniform("model", transform);

				glActiveTexture(GL_TEXTURE0 + nat);
				entity->second.GetComponent<TerrainComponent>().GetHeightMapTexture().bind();
				entity->second.GetComponent<TerrainComponent>().GetShader().setUniform("heightMap", nat);
				nat++;

				//glActiveTexture(GL_TEXTURE0 + nat);
				//entity->second.GetComponent<MaterialComponent>().GetMaterial().getTexture("albedo")->bind();
				//entity->second.GetComponent<TerrainComponent>().GetShader().setUniform("uTexture", nat);
				//nat++;

				entity->second.GetComponent<TerrainComponent>().Draw();

				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

				m_SceneData.m_Shader.use();
			}
		}

		//m_SceneData.m_Shader.setUniform("uUseDirLight", dirLightCount);
		m_SceneData.m_Shader.setUniform("uUsePointLight", pointLightCount);

		m_SceneData.m_Scene->getSkybox().GetShader()->use();
		m_SceneData.m_Scene->getSkybox().GetShader()->setUniform("projection", projectionMatrix);
		m_SceneData.m_Scene->getSkybox().GetShader()->setUniform("view", viewMatrix);

		glActiveTexture(GL_TEXTURE0);
		m_SceneData.m_Scene->getSkybox().BindCubeMap();
		m_SceneData.m_Scene->getSkybox().GetModel()->draw();
	}

	void Renderer::EndScene()
	{

	}

	void Renderer::Clear() {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void Renderer::ClearColor(const glm::vec4& color) {
		glClearColor(color.r, color.g, color.b, color.a);
	}

	void Renderer::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
	{
		glViewport(x, y, width, height);
	}

	double Renderer::GetTime()
	{
		return glfwGetTime();
	}
}