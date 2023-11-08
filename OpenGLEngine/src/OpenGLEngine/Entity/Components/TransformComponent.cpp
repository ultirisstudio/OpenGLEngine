#include "depch.h"
#include "TransformComponent.h"

TransformComponent::TransformComponent(const glm::vec3& position) : Position(position) {}

glm::mat4 TransformComponent::GetTransform() const
{
	glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), glm::radians(Rotation.x), { 1, 0, 0 }) * glm::rotate(glm::mat4(1.0f), glm::radians(Rotation.y), { 0, 1, 0 }) * glm::rotate(glm::mat4(1.0f), glm::radians(Rotation.z), { 0, 0, 1 });
	return glm::translate(glm::mat4(1.f), Position) * rotation * glm::scale(glm::mat4(1.0f), Scale);
}

glm::quat TransformComponent::GetQuatRotation() const
{
	glm::quat rotation = glm::quat(Rotation);
	return rotation;
}
