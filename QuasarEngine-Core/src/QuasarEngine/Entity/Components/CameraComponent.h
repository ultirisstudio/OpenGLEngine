#pragma once

#include <QuasarEngine/Entity/Component.h>
#include <QuasarEngine/Scene/Camera.h>

namespace QuasarEngine
{
	class CameraComponent : public Component
	{
	private:
		std::unique_ptr<Camera> m_Camera;
	public:
		CameraComponent();

		Camera& GetCamera() { return *m_Camera; }

		bool Primary = false;

		enum class CameraType
		{
			PERSPECTIVE = 0,
			ORTHOGRAPHIC
		};

		const char* item_type = "Perspective";
		CameraType cameraType = CameraType::PERSPECTIVE;

		void setType(CameraType type);
	};
}