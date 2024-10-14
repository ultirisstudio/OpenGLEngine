#include "depch.h"
#include "CameraComponent.h"

#include <OpenGLEngine/ECS/Entity.h>
#include <OpenGLEngine/ECS/Components/TransformComponent.h>

namespace OpenGLEngine
{
	CameraComponent::CameraComponent() : m_Camera(std::make_unique<Camera>())
	{
		
	}
}