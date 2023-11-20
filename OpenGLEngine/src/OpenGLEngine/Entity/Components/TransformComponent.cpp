#include "depch.h"
#include "TransformComponent.h"

namespace OpenGLEngine
{
	TransformComponent::TransformComponent(const glm::vec3& position) : Position(position) {}

	glm::mat4 TransformComponent::GetTransform() const
	{
		glm::mat4 rotation = glm::toMat4(glm::quat(Rotation));

		return glm::translate(glm::mat4(1.0f), Position)
			* rotation
			* glm::scale(glm::mat4(1.0f), Scale);
	}
}