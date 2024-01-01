#include "depch.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>

#include <OpenGLEngine/Core/Application.h>
#include <OpenGLEngine/Renderer/Renderer.h>
#include <OpenGLEngine/Shader/Generators/ShaderGenerator.h>
#include <OpenGLEngine/Tools/Log.h>

#include <OpenGLEngine/Entity/Components/TransformComponent.h>
#include <OpenGLEngine/Entity/Components/ModelComponent.h>
#include <OpenGLEngine/Entity/Components/MaterialComponent.h>
#include <OpenGLEngine/Entity/Components/SkyboxComponent.h>
#include <OpenGLEngine/Entity/Components/LightComponent.h>
#include <OpenGLEngine/Entity/Components/TerrainComponent.h>

#include <OpenGLEngine/Tools/Math.h>

namespace OpenGLEngine {
	Renderer::SceneData Renderer::m_SceneData = Renderer::SceneData();

	void Renderer::Init()
	{
		m_SceneData.m_Shader = Shader();
		m_SceneData.m_Shader.LoadFromFile("Shaders/basic.vert", "Shaders/basic.frag");
	}

	void Renderer::BeginScene(Scene& scene)
	{
		m_SceneData.m_Scene = &scene;
	}

	void Renderer::Render(bool runtime)
	{
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

		m_SceneData.m_Shader.use();

		int dirLightCount = 0;
		int pointLightCount = 0;

		for (auto entity = m_SceneData.m_Scene->getEntities()->begin(); entity != m_SceneData.m_Scene->getEntities()->end(); entity++)
		{
			if (entity->second.HasComponent<LightComponent>())
			{
				auto& lc = entity->second.GetComponent<LightComponent>();
				if (lc.lightType == LightComponent::LightType::DIRECTIONAL)
				{
					m_SceneData.m_Shader.setUniform("dirLights[" + std::to_string(dirLightCount) + "].direction", entity->second.GetComponent<TransformComponent>().Rotation);
					m_SceneData.m_Shader.setUniform("dirLights[" + std::to_string(dirLightCount) + "].diffuse", lc.dir_diffuse);
					m_SceneData.m_Shader.setUniform("dirLights[" + std::to_string(dirLightCount) + "].specular", lc.dir_specular);

					dirLightCount++;
				}
				else if (lc.lightType == LightComponent::LightType::POINT)
				{
					m_SceneData.m_Shader.setUniform("pointLights[" + std::to_string(pointLightCount) + "].position", entity->second.GetComponent<TransformComponent>().Position);
					m_SceneData.m_Shader.setUniform("pointLights[" + std::to_string(pointLightCount) + "].diffuse", lc.point_diffuse);
					m_SceneData.m_Shader.setUniform("pointLights[" + std::to_string(pointLightCount) + "].specular", lc.point_specular);
					m_SceneData.m_Shader.setUniform("pointLights[" + std::to_string(pointLightCount) + "].constant", lc.point_constant);
					m_SceneData.m_Shader.setUniform("pointLights[" + std::to_string(pointLightCount) + "].linear", lc.point_linear);
					m_SceneData.m_Shader.setUniform("pointLights[" + std::to_string(pointLightCount) + "].quadratic", lc.point_quadratic);

					pointLightCount++;
				}
			}

			if (((entity->second.HasComponent<ModelComponent>() && entity->second.GetComponent<ModelComponent>().GetPtr()) || entity->second.HasComponent<TerrainComponent>()) && entity->second.HasComponent<MaterialComponent>())
			{
				Material& material = entity->second.GetComponent<MaterialComponent>().GetMaterial();
				glm::mat4& transform = entity->second.GetComponent<TransformComponent>().GetTransform();

				int nat = 0;

				m_SceneData.m_Shader.setUniform("uModel", transform);
				m_SceneData.m_Shader.setUniform("uView", viewMatrix);
				m_SceneData.m_Shader.setUniform("uProjection", projectionMatrix);

				m_SceneData.m_Shader.setUniform("uMaterial.ambient_color", *material.getVec3("ambient"));
				m_SceneData.m_Shader.setUniform("uMaterial.diffuse_color", *material.getVec3("diffuse"));
				m_SceneData.m_Shader.setUniform("uMaterial.specular_color", *material.getVec3("specular"));
				m_SceneData.m_Shader.setUniform("uMaterial.shininess", *material.getFloat("shininess"));

				m_SceneData.m_Shader.setUniform("uMaterial.use_diffuse_texture", *material.getBoolean("diffuse"));
				m_SceneData.m_Shader.setUniform("uMaterial.use_specular_texture", *material.getBoolean("specular"));

				if (*material.getBoolean("diffuse"))
				{
					glActiveTexture(GL_TEXTURE0 + nat);
					material.getTexture("diffuse")->bind();
					m_SceneData.m_Shader.setUniform("uMaterial.diffuse_texture", nat);
					nat++;
				}

				if (*material.getBoolean("specular"))
				{
					glActiveTexture(GL_TEXTURE0 + nat);
					material.getTexture("specular")->bind();
					m_SceneData.m_Shader.setUniform("uMaterial.specular_texture", nat);
					nat++;
				}

				if (entity->second.HasComponent<TerrainComponent>())
				{
					if (entity->second.GetComponent<TerrainComponent>().m_PolygonMode)
						glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

					entity->second.GetComponent<TerrainComponent>().Draw();

					glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
				}
				
				if (entity->second.HasComponent<ModelComponent>())
					entity->second.GetComponent<ModelComponent>().GetModel().draw();
			}

			if (entity->second.HasComponent<SkyboxComponent>())
			{
				auto& sc = entity->second.GetComponent<SkyboxComponent>();

				sc.GetCubeMap()->ActiveTexture();
				sc.GetCubeMap()->Bind();
				sc.GetCubeMapShader()->use();
				sc.GetCubeMapShader()->setUniform("uView", glm::mat4(glm::mat3(viewMatrix)));
				sc.GetCubeMapShader()->setUniform("uProjection", projectionMatrix);
				sc.GetCubeMapShader()->setUniform("uCubeMap", 0);
				sc.GetCubeMap()->BeginDrawModel();
				sc.GetModel()->draw();
				sc.GetCubeMap()->EndDrawModel();
			}
		}

		m_SceneData.m_Shader.setUniform("uUseDirLight", dirLightCount);
		m_SceneData.m_Shader.setUniform("uUsePointLight", pointLightCount);
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