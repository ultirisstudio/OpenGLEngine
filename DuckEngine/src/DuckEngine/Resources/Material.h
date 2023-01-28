#pragma once

#include <DuckEngine/Resources/Resource.h>

#include <glm/glm.hpp>

#include <map>

namespace DuckEngine
{
	class Material : public Resource
	{
	private:
		std::map<std::string, glm::vec3> m_vec3s;
		std::map<std::string, float> m_floats;

	public:
		void load(const std::string& path) override;
		void free() override;

		void addVec3(const std::string& id, const glm::vec3& value);
		void addFloat(const std::string& id, float value);
		
		glm::vec3 getVec3(const std::string& id);
		float getFloat(const std::string& id);
	};
}