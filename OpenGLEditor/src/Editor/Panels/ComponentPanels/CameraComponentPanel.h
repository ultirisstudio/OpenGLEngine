#pragma once


#include "../../SceneManager.h"

namespace OpenGLEngine
{
	class Entity;

	class CameraComponentPanel
	{
	public:
		CameraComponentPanel() = default;
		~CameraComponentPanel() = default;

		void Render(Entity entity, SceneManager& sceneManager);
	};
}
