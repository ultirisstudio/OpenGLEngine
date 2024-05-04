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
	ModelComponent::ModelComponent() : m_Model(nullptr)
	{
		
	}

	void ModelComponent::SetModel(const std::string& path)
	{
		m_Model = OpenGLEngine::Renderer::m_SceneData.m_ResourceManager.getModel(path);
		m_ModelPath = path;

		for (auto& [name, mesh] : m_Model->GetMeshes())
		{
			Entity* entity = Renderer::GetScene()->CreateEntity(name);

			this->entity->AddChild(entity->GetUUID());

			entity->AddComponent<MeshComponent>(name, mesh, path);
			entity->AddComponent<MaterialComponent>();
		}
	}
}