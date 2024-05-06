#pragma once

#include <glm/glm.hpp>

namespace OpenGLEngine
{
	struct DebugLineVertex
	{
		glm::vec3 position;
		glm::vec3 color;
	};

	struct DebugTriangleVertex
	{
		glm::vec3 position;
		glm::vec3 color;
	};
}