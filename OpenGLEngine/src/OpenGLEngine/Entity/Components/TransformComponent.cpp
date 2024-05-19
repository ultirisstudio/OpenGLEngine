#include "depch.h"
#include "TransformComponent.h"

#include <OpenGLEngine/Entity/Entity.h>
#include <OpenGLEngine/Renderer/Renderer.h>

namespace OpenGLEngine
{
	TransformComponent::TransformComponent(const glm::vec3& position) : Position(position) {}

	glm::mat4 TransformComponent::GetTransform() const
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), Position) * glm::toMat4(glm::quat(Rotation)) * glm::scale(glm::mat4(1.0f), Scale);

		return transform;
	}
}