#pragma once

#include <OpenGLEngine/Entity/Component.h>
#include <OpenGLEngine/Scene/Camera.h>

namespace OpenGLEngine
{
	class CameraComponent : public Component
	{
	private:
		std::unique_ptr<Camera> m_Camera;
	public:
		CameraComponent();

		Camera& GetCamera() { return *m_Camera; }
	};
}