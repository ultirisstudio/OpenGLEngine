#include "depch.h"
#include "ModelComponent.h"

#include <OpenGLEngine/Renderer/Renderer.h>
#include <OpenGLEngine/Resources/Mesh.h>
#include <OpenGLEngine/Scene/Scene.h>

namespace OpenGLEngine
{
	ModelComponent::ModelComponent(const std::string& path)
	{
		m_Model = OpenGLEngine::Renderer::m_SceneData.m_ResourceManager.getModel(path);
		m_ModelPath = path;
	}

	void ModelComponent::SetModel(const std::string& path)
	{
		m_Model = OpenGLEngine::Renderer::m_SceneData.m_ResourceManager.getModel(path);
		m_ModelPath = path;

		/*m_SubEntities.clear();
		for (auto& [name, mesh] : m_Model->GetMeshes())
		{
			m_SubEntities.push_back(scene->CreateEntity(name));
			//m_SubEntities.back()->AddComponent<>();
			//m_SubEntities.back().AddComponent<MeshComponent>(mesh.second);
		}*/
	}
}