#include "depch.h"
#include "HierarchyComponent.h"

#include <OpenGLEngine/ECS/Entity.h>
#include <OpenGLEngine/Renderer/Renderer.h>
#include <OpenGLEngine/Scene/Scene.h>

namespace OpenGLEngine
{
	void HierarchyComponent::AddChild(UUID parent, UUID child)
	{
		m_Childrens.emplace_back(child);
		Entity entity = OpenGLEngine::Renderer::m_SceneData.m_Scene->GetEntityByUUID(child);
		entity.GetComponent<HierarchyComponent>().m_Parent = parent;
	}
}