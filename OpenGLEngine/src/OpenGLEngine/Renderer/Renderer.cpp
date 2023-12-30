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

#include <OpenGLEngine/Tools/Math.h>

namespace OpenGLEngine {
	Renderer::SceneData Renderer::m_SceneData = Renderer::SceneData();

	void Renderer::Init()
	{
		m_SceneData.m_Shader = Shader();
		m_SceneData.m_Shader.LoadFromFile("Shaders/basic.vert", "Shaders/basic.frag");

		m_SceneData.m_Shader.setUniform("uLight.position", glm::vec3(1.0f, 1.0f, 1.0f));
		m_SceneData.m_Shader.setUniform("uLight.diffuse", glm::vec3(1.0f, 1.0f, 1.0f));
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

		for (auto entity = m_SceneData.m_Scene->getEntities()->begin(); entity != m_SceneData.m_Scene->getEntities()->end(); entity++)
		{
			if (entity->second.HasComponent<LightComponent>())
			{
				m_SceneData.m_Shader.setUniform("uLight.position", entity->second.GetComponent<TransformComponent>().Position);
				m_SceneData.m_Shader.setUniform("uLight.diffuse", entity->second.GetComponent<LightComponent>().diffuse);
			}

			if (entity->second.HasComponent<ModelComponent>() && entity->second.HasComponent<MaterialComponent>() && entity->second.GetComponent<ModelComponent>().GetPtr())
			{
				Material& material = entity->second.GetComponent<MaterialComponent>().GetMaterial();
				Model& model = entity->second.GetComponent<ModelComponent>().GetModel();
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

				model.draw();
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