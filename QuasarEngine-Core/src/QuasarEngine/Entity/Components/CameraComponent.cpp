#include "qepch.h"
#include "CameraComponent.h"

#include <QuasarEngine/Entity/Entity.h>
#include <QuasarEngine/Entity/Components/TransformComponent.h>

namespace QuasarEngine
{
	CameraComponent::CameraComponent() : m_Camera(std::make_unique<Camera>())
	{
		
	}
}