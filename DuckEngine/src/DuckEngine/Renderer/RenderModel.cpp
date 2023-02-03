#include "depch.h"
#include <DuckEngine/Renderer/RenderModel.h>
#include "DuckEngine/Core/Application.h"
#include "glad/glad.h"

DuckEngine::RenderModel::RenderModel(Material& material, Shader& shader, Model& model) : Element(material, shader), m_Model(&model)
{

}

void DuckEngine::RenderModel::SetUniforms()
{
	m_Shader->use();

	m_Shader->setUniform("uModel", m_ModelMatrix);
	m_Shader->setUniform("uView", Renderer::getViewMatrix());
	m_Shader->setUniform("uProjection", Renderer::getProjectionMatrix());
}

void DuckEngine::RenderModel::BindTexture()
{
	glActiveTexture(GL_TEXTURE0);
}

void DuckEngine::RenderModel::draw() const
{
	m_Model->draw();
}
