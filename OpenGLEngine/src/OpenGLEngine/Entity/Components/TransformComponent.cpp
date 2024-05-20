#include "depch.h"
#include "TransformComponent.h"

#include <OpenGLEngine/Entity/Entity.h>
#include <OpenGLEngine/Renderer/Renderer.h>

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtx/euler_angles.hpp>

namespace OpenGLEngine
{
	TransformComponent::TransformComponent(const glm::vec3& position) : Position(position) {}

	glm::mat4 TransformComponent::GetLocalTransform() const
	{
		return glm::translate(glm::mat4(1.0f), Position) * glm::toMat4(glm::quat(Rotation)) * glm::scale(glm::mat4(1.0f), Scale);
	}

	glm::mat4 TransformComponent::GetGlobalTransform() const
	{
		glm::mat4 finalTransform;
		glm::mat4& transform = glm::translate(glm::mat4(1.0f), Position) * glm::toMat4(glm::quat(Rotation)) * glm::scale(glm::mat4(1.0f), Scale);

		std::vector<glm::mat4> transforms;
		UUID parentID = Component::entity->m_Parent;

		if (parentID == UUID::Null())
		{
			return transform;
		}
		else
		{
			while (parentID != UUID::Null())
			{
				Entity* parent = &Renderer::m_SceneData.m_Scene->getEntities()->find(parentID)->second;
				if (parent)
				{
					glm::mat4 parentTransform = parent->GetComponent<TransformComponent>().GetLocalTransform();

					transforms.push_back(parentTransform);

					parentID = parent->m_Parent;
				}
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
	}

	glm::mat4 TransformComponent::CalculateViewMatrix(glm::mat4 transform) const
	{
		glm::mat4 viewMatrix;

		glm::vec3 m_target;
		glm::vec3 m_up;
		glm::vec3 m_right;
		glm::vec3 m_worldUp;

		glm::vec3 front;

		glm::vec3 position, scale;
		glm::quat rotationQuat;
		glm::decompose(transform, scale, rotationQuat, position, glm::vec3(), glm::vec4());
		glm::vec3 rotation = glm::eulerAngles(rotationQuat);

		front.x = cos(rotation.y) * cos(rotation.x);
		front.y = sin(rotation.x);
		front.z = sin(rotation.y) * cos(rotation.x);
		m_target = glm::normalize(front);
		m_right = glm::normalize(glm::cross(m_target, m_worldUp));
		m_up = glm::normalize(glm::cross(m_right, m_target));

		return glm::lookAt(position, position + m_target, glm::vec3(0.0f, 1.0f, 0.0f));
	}

	glm::mat4 TransformComponent::GetLocalViewMatrix() const
	{
		//return glm::inverse(GetLocalTransform());
		return CalculateViewMatrix(GetLocalTransform());
	}

	glm::mat4 TransformComponent::GetGlobalViewMatrix() const
	{
		//return glm::inverse(GetGlobalTransform());
		return CalculateViewMatrix(GetGlobalTransform());
	}
}