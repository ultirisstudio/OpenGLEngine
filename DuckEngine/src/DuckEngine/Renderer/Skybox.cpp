#include "depch.h"
#include "DuckEngine.h"
#include "Skybox.h"
#include "glad/glad.h"
/*
namespace DuckEngine
{
	Skybox::Skybox(Model& model) : m_CubeMapShader()
	{
		m_CubeMapShader.LoadFromFile("Shaders/cubemap.vert", "Shaders/cubemap.frag");
		m_CubeMapModel = Renderer::CreateRenderModel(&model, &m_CubeMapShader, nullptr);
		//m_CubeMapModel->scale(glm::vec3(15.0f));

		m_CubeMap.Load({ "Assets/Skybox/right.jpg", "Assets/Skybox/left.jpg", "Assets/Skybox/top.jpg", "Assets/Skybox/bottom.jpg", "Assets/Skybox/front.jpg", "Assets/Skybox/back.jpg" });
	}

	void DuckEngine::Skybox::Draw()
	{
		glActiveTexture(GL_TEXTURE0);
		m_CubeMap.Bind();
		m_CubeMapModel->SetUniforms();
		m_CubeMapShader.setUniform("uCubeMap", 0);
		m_CubeMapShader.setUniform("uView", glm::mat4(glm::mat3(Renderer::getViewMatrix())));
		glDepthFunc(GL_LEQUAL);
		m_CubeMapModel->draw();
		glDepthFunc(GL_LESS);
	}
}
*/