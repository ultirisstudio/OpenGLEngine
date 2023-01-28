#include "depch.h"
#include <DuckEngine/Resources/Material.h>

#include <DuckEngine/Tools/Log.h>

#include <fstream>
#include <sstream>

void DuckEngine::Material::load(const std::string& path)
{
	if (path.empty())
		return;

	std::ifstream file(path);
	if (!file.is_open())
		Log::error("Failed to load Material at " + path);

	std::stringstream stream;
	stream << file.rdbuf();

	file.close();

	std::string line;
	while (std::getline(stream, line, ' '))
	{
		if (line == "vec3")
		{
			std::getline(stream, line, ' ');

			std::string id = line;
			std::getline(stream, line, ' ');
			std::getline(stream, line, ',');

			glm::vec3 value;
			value.x = std::stof(line);
			std::getline(stream, line, ' ');
			std::getline(stream, line, ',');
			value.y = std::stof(line);
			std::getline(stream, line, ' ');
			std::getline(stream, line, '\n');
			value.z = std::stof(line);

			m_vec3s[id] = value;
		}
		else if (line == "float")
		{
			std::getline(stream, line, ' ');

			std::string id = line;
			std::getline(stream, line, ' ');
			std::getline(stream, line, '\n');

			float value = std::stof(line);
			m_floats[id] = value;
		}
	}
}

void DuckEngine::Material::free()
{

}

void DuckEngine::Material::addVec3(const std::string& id, const glm::vec3& value)
{
	m_vec3s[id] = value;
}

void DuckEngine::Material::addFloat(const std::string& id, float value)
{
	m_floats[id] = value;
}

glm::vec3 DuckEngine::Material::getVec3(const std::string& id)
{
	return m_vec3s.at(id);
}

float DuckEngine::Material::getFloat(const std::string& id)
{
	return m_floats.at(id);
}
