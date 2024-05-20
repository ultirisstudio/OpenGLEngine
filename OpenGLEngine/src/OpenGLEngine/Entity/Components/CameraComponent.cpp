#include "depch.h"
#include "CameraComponent.h"

#include <OpenGLEngine/Entity/Entity.h>
#include <OpenGLEngine/Entity/Components/TransformComponent.h>

namespace OpenGLEngine
{
	CameraComponent::CameraComponent() : m_Camera(std::make_unique<Camera>())
	{
		
	}

	void CameraComponent::Init()
	{
		if (this->entity)
		{
			if (this->entity->HasComponent<TransformComponent>())
			{
				auto& tc = this->entity->GetComponent<TransformComponent>();
				m_Camera->Init(tc.Position, tc.Rotation);
			}
		}
	}
}