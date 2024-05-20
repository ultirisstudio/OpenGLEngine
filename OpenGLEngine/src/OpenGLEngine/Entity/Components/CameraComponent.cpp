#include "depch.h"
#include "CameraComponent.h"

#include <OpenGLEngine/Entity/Entity.h>
#include <OpenGLEngine/Entity/Components/TransformComponent.h>

namespace OpenGLEngine
{
	CameraComponent::CameraComponent() : m_Camera(std::make_unique<Camera>())
	{
		
	}
}