#include "depch.h"
#include "ModelComponent.h"

#include <OpenGLEngine/Renderer/Renderer.h>
#include <OpenGLEngine/Scene/Scene.h>
#include <OpenGLEngine/Entity/Entity.h>

#include <OpenGLEngine/Core/UUID.h>

#include <OpenGLEngine/Entity/Components/TransformComponent.h>
#include <OpenGLEngine/Entity/Components/MeshComponent.h>
#include <OpenGLEngine/Entity/Components/MaterialComponent.h>

namespace OpenGLEngine
{
	ModelComponent::ModelComponent(const std::string& path)
	{
		SetModel(path);
	}

	void ModelComponent::SetModel(const std::string& path)
	{
		m_Model = OpenGLEngine::Renderer::m_SceneData.m_ResourceManager.getModel(path);
		m_ModelPath = path;

		m_SubEntities.clear();
		for (auto& [name, mesh] : m_Model->GetMeshes())
		{
			m_SubEntities.push_back(Entity(name, UUID()));
			m_SubEntities.back().AddComponent<TransformComponent>();
			m_SubEntities.back().AddComponent<MeshComponent>(name, mesh);
			m_SubEntities.back().AddComponent<MaterialComponent>();
		}
	}
}