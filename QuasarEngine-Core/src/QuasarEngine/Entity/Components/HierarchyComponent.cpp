#include "qepch.h"
#include "HierarchyComponent.h"

#include <QuasarEngine/Entity/Entity.h>
#include <QuasarEngine/Renderer/Renderer.h>
#include <QuasarEngine/Scene/Scene.h>

namespace QuasarEngine
{
	void HierarchyComponent::AddChild(UUID parent, UUID child)
	{
		m_Childrens.emplace_back(child);
		Entity entity = QuasarEngine::Renderer::m_SceneData.m_Scene->GetEntityByUUID(child);
		entity.GetComponent<HierarchyComponent>().m_Parent = parent;
	}
}