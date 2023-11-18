#include "depch.h"
#include "ModelComponent.h"

ModelComponent::ModelComponent(const std::string& path)
{
	m_Model = OpenGLEngine::Renderer::m_SceneData.m_ResourceManager.getModel(path);
	m_ModelPath = path;
}

void ModelComponent::SetModel(const std::string& path)
{
	m_Model = OpenGLEngine::Renderer::m_SceneData.m_ResourceManager.getModel(path);
	m_ModelPath = path;
}
