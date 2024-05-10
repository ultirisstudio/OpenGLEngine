#include "depch.h"
#include "Entity.h"

#include <OpenGLEngine/Renderer/Renderer.h>
#include <OpenGLEngine/Scene/Scene.h>

Entity::Entity(std::string name, OpenGLEngine::UUID uuid) : m_Name(name), m_UUID(uuid)
{

}

void Entity::AddChild(const OpenGLEngine::UUID& id)
{
	m_Children.emplace_back(id);
	OpenGLEngine::Renderer::m_SceneData.m_Scene->GetEntityByUUID(id)->m_Parent = m_UUID;
}
