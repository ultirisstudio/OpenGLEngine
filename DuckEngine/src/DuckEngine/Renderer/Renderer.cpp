#include "depch.h"
#include <DuckEngine/Renderer/Renderer.h>
#include <glad/glad.h>
#include <DuckEngine/Tools/Log.h>
#include <glm/gtc/matrix_transform.hpp>
#include "DuckEngine/Core/Application.h"

namespace DuckEngine {
	Renderer::SceneData Renderer::m_SceneData = Renderer::SceneData();

	Shader* Renderer::CreateShader(Material& material)
	{
		return m_SceneData.m_shaderGenerator.generateShader(material);
	}

	const glm::mat4& Renderer::getViewMatrix()
	{
		return m_SceneData.m_viewMatrix;
	}

	const glm::mat4& Renderer::getProjectionMatrix()
	{
		return m_SceneData.m_projectionMatrix;
	}

	void Renderer::BeginScene(Camera* camera)
	{
		m_SceneData.m_viewMatrix = camera->getViewMatrix();
		m_SceneData.m_projectionMatrix = camera->getProjectionMatrix();
	}

	void Renderer::UpdateMatrix(glm::mat4 viewMatrix, glm::mat4 projectionMatrix)
	{
		m_SceneData.m_viewMatrix = viewMatrix;
		m_SceneData.m_projectionMatrix = projectionMatrix;
	}

	void Renderer::EndScene()
	{
	}

	//std::shared_ptr<RenderModel> Renderer::CreateRenderModel(Model* model, Material* material)
	//{
	//	return std::make_shared<RenderModel>(*material, *CreateShader(*material), *model);
	//}

	std::shared_ptr<RenderModel> Renderer::CreateRenderModel(Model* model, Shader* shader, Material* material)
	{
		return std::make_shared<RenderModel>(*material, *shader, *model);
	}

	std::shared_ptr<Skybox> Renderer::CreateSkybox(Model& mode)
	{
		return std::make_shared<Skybox>(mode);
	}

	std::shared_ptr<Texture> Renderer::CreateTexture(const std::string& path)
	{
		return std::make_shared<Texture>(path);
	}

	std::shared_ptr<Model> Renderer::CreateModel(const std::string& path)
	{
		return std::make_shared<Model>(path);
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
}