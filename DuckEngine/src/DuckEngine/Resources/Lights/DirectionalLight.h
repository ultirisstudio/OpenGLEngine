#pragma once

#include <glm/glm.hpp>

namespace DuckEngine
{
	struct DirectionalLight
	{
		glm::vec3 ambient = glm::vec3(0.1f);
		glm::vec3 diffuse = glm::vec3(0.8f);
		glm::vec3 specular = glm::vec3(1.0f);

		glm::vec3 direction = glm::vec3(-1.0f);
	};
}