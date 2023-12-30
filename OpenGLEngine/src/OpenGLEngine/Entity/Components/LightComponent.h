#pragma once

#include <OpenGLEngine/Entity/Component.h>

namespace OpenGLEngine
{
	class LightComponent : public Component
	{
	public:
		glm::vec3 diffuse = { 1.0f, 1.0f, 1.0f };

		LightComponent() {}
	};
}