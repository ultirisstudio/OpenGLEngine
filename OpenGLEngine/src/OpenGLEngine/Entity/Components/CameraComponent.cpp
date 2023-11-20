#include "depch.h"
#include "CameraComponent.h"

#include <OpenGLEngine/Renderer/Renderer.h>
#include <OpenGLEngine/Entity/Components/TransformComponent.h>

namespace OpenGLEngine
{
	CameraComponent::CameraComponent() :
		m_Camera(std::make_unique<Camera>(glm::vec3(0.0f, 0.0f, 0.0f)))
	{
		
	}
}