#include "depch.h"
#include "CameraComponent.h"

namespace OpenGLEngine
{
	CameraComponent::CameraComponent() : m_Camera(std::make_unique<Camera>(glm::vec3(0.0f, 0.0f, 0.0f)))
	{

	}
}