#pragma once

#include <OpenGLEngine/Entity/Component.h>

class TransformComponent : public Component
{
public:
	glm::vec3 Position = { 0.0f,0.0f,0.0f };
	glm::vec3 Rotation = { 0.0f,0.0f,0.0f };
	glm::vec3 Scale = { 1.0f,1.0f,1.0f };

	TransformComponent() = default;
	TransformComponent(const glm::vec3& position);

	void SetPosition(const glm::vec3& position) { Position = position; }
	void SetRotation(const glm::vec3& rotation) { Rotation = rotation; }
	void SetScale(const glm::vec3& scale) { Scale = scale; }

	glm::mat4 GetTransform() const;
	glm::quat GetQuatRotation() const;
};