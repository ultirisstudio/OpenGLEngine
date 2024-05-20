#include "depch.h"
#include "TransformComponent.h"

#include <OpenGLEngine/Entity/Entity.h>
#include <OpenGLEngine/Renderer/Renderer.h>

namespace OpenGLEngine
{
	TransformComponent::TransformComponent(const glm::vec3& position) : Position(position) {}

	glm::mat4 TransformComponent::GetTransform() const
	{
		//for (auto& e : *Renderer::m_SceneData.m_Scene->getEntities())
			//std::cout << "Renderer: " << e.second.GetName() << " : " << e.second.GetUUID() << std::endl;

		std::cout << "Component: " << this->entity->GetUUID() << std::endl;

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), Position) * glm::toMat4(glm::quat(Rotation)) * glm::scale(glm::mat4(1.0f), Scale);

		return transform;

		/*
		glm::mat4 finalTransform;
		glm::mat4& transform = glm::translate(glm::mat4(1.0f), Position) * glm::toMat4(glm::quat(Rotation)) * glm::scale(glm::mat4(1.0f), Scale);

		std::vector<glm::mat4> transforms;
		UUID parentID = Component::entity->m_Parent;

		if (parentID == UUID::Null())
			finalTransform = transform;

		while (parentID != UUID::Null())
		{
			Entity* parent = &Renderer::m_SceneData.m_Scene->getEntities()->find(parentID)->second;
			if (parent)
			{
				glm::mat4 parentTransform = parent->GetComponent<TransformComponent>().GetTransform();

				transforms.push_back(parentTransform);

				parentID = parent->m_Parent;
			}

			finalTransform = transforms[transforms.size() - 1];

			if (transforms.size() > 1)
			{
				for (int i = transforms.size() - 2; i >= 0; i--)
				{
					finalTransform *= transforms[i];
				}
			}

			finalTransform *= transform;
		}

		return finalTransform;
		*/
	}
}