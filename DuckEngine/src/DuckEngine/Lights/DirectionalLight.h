#pragma once

#include <DuckEngine/Resources/Resource.h>

#include <glm/glm.hpp>

namespace DuckEngine
{
	struct DirectionalLight : public Resource
	{
		glm::vec3 ambient = glm::vec3(0.1f);
		glm::vec3 diffuse = glm::vec3(0.8f);
		glm::vec3 specular = glm::vec3(1.0f);

		glm::vec3 direction = glm::vec3(-1.0f);

		void load(const std::string& path) override;
		void free() override;
	};
}