#include "depch.h"

#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>

#include <DuckEngine/Core/Application.h>
#include <DuckEngine/Renderer/Renderer.h>
#include <DuckEngine/Shader/Generators/ShaderGenerator.h>
#include <DuckEngine/Tools/Log.h>

namespace DuckEngine {
	Renderer::SceneData Renderer::m_SceneData = Renderer::SceneData();

	Shader* Renderer::CreateShader(Material& material)
	{
		ShaderGenerator shaderGenerator(material, ShaderType::BPhong);
		Shader shader;
		std::cout << shaderGenerator.generateVertexShader() << std::endl;
		std::cout << shaderGenerator.generateFragmentShader() << std::endl;
		shader.LoadFromSource(shaderGenerator.generateVertexShader(), shaderGenerator.generateFragmentShader(), shaderGenerator.getVertexShaderRenderInfo(), shaderGenerator.getFragmentShaderRenderInfo());
		return &shader;
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

	std::shared_ptr<Texture> Renderer::CreateTexture(const std::string& path)
	{
		return std::make_shared<Texture>(path);
	}

	std::shared_ptr<Model> Renderer::CreateModel(const std::string& path)
	{
		return std::make_shared<Model>(path);
	}

	std::shared_ptr<Material> Renderer::CreateMaterial()
	{
		return std::make_shared<Material>();
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