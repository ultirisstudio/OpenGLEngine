#pragma once

#include <glm/glm.hpp>

namespace OpenGLEngine
{
	struct PointLight
	{
		glm::vec3* ambient;
		glm::vec3* diffuse;
		glm::vec3* specular;

		glm::vec3* position;

		float* constant;
		float* linear;
		float* quadratic;
	};
}