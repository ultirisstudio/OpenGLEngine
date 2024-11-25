#include "qepch.h"
#include "CameraComponent.h"

#include <QuasarEngine/Entity/Entity.h>
#include <QuasarEngine/Entity/Components/TransformComponent.h>

namespace QuasarEngine
{
	CameraComponent::CameraComponent() : m_Camera(std::make_unique<Camera>())
	{
		
	}

	void CameraComponent::setType(CameraType type)
	{
		switch (type)
		{
		case CameraType::PERSPECTIVE:
			cameraType = CameraComponent::CameraType::PERSPECTIVE;
			item_type = "Perspective";
			break;
		case CameraType::ORTHOGRAPHIC:
			cameraType = CameraComponent::CameraType::ORTHOGRAPHIC;
			item_type = "Orthographic";
			break;
		default:
			cameraType = CameraComponent::CameraType::PERSPECTIVE;
			item_type = "Perspective";
			break;
		}
	}
}