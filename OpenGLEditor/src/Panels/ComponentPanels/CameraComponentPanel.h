#pragma once

class Entity;

#include "../../SceneManager.h"

namespace OpenGLEngine
{
	class CameraComponentPanel
	{
	public:
		CameraComponentPanel() = default;
		~CameraComponentPanel() = default;

		void Render(Entity* entity, SceneManager& sceneManager);
	};
}
