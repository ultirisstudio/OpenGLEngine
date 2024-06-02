#include "depch.h"
#include "Entity.h"

#include <OpenGLEngine/Renderer/Renderer.h>
#include <OpenGLEngine/Scene/Scene.h>

#include <OpenGLEngine/Entity/Components/Physics/RigidBodyComponent.h>

Entity::Entity(std::string name) : m_Name(name)
{

}

void Entity::Destroy()
{
	if (HasComponent<OpenGLEngine::RigidBodyComponent>())
	{
		GetComponent<OpenGLEngine::RigidBodyComponent>().Destroy();
	}
}

void Entity::AddChild(const OpenGLEngine::UUID& id)
{
	m_Children.emplace_back(id);
	Entity* entity = OpenGLEngine::Renderer::m_SceneData.m_Scene->GetEntityByUUID(id);
	if (entity != nullptr)
		entity->m_Parent = GetUUID();
}