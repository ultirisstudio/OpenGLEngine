#include "depch.h"

#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>

#include <DuckEngine/Core/Application.h>
#include <DuckEngine/Renderer/Renderer.h>
#include <DuckEngine/Shader/Generators/ShaderGenerator.h>
#include <DuckEngine/Tools/Log.h>

namespace DuckEngine {
	Renderer::SceneData Renderer::m_SceneData = Renderer::SceneData();

	void Renderer::BeginScene(Camera* camera)
	{
		m_SceneData.m_ActiveCamera = camera;
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
}