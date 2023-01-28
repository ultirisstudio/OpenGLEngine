#include "depch.h"
#include <DuckEngine/Shader/Shader.h>
#include <DuckEngine/Tools/Log.h>
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

unsigned int DuckEngine::Shader::createShader(const std::string& source, unsigned int type)
{
	const char* src = source.c_str();

	unsigned int shader;
	shader = glCreateShader(type);

	glShaderSource(shader, 1, &src, NULL);
	glCompileShader(shader);

	int compileStatus;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compileStatus);
	if (!compileStatus)
	{
		char error[512];
		glGetShaderInfoLog(shader, 512, NULL, error);

		std::string shaderType;
		switch (type)
		{
		case GL_VERTEX_SHADER:
			shaderType = "vertex";
			break;
		case GL_FRAGMENT_SHADER:
			shaderType = "fragment";
			break;
		case GL_GEOMETRY_SHADER:
			shaderType = "geometry";
			break;
		default:
			break;
		}

		std::string errorMessage("Failed to compile " + shaderType + " shader : ");
		Log::error(errorMessage + error);

		return 0;
	}

	return shader;
}

DuckEngine::Shader::Shader(const std::string& vs, const std::string& fs, const std::string& gs) :
	m_id(0)
{
	if (vs.empty() || fs.empty())
		return;

	unsigned int vertexShader = createShader(vs, GL_VERTEX_SHADER);
	unsigned int geometryShader = gs.empty() ? 0 : createShader(gs, GL_GEOMETRY_SHADER);
	unsigned int fragmentShader = createShader(fs, GL_FRAGMENT_SHADER);

	m_id = glCreateProgram();

	glAttachShader(m_id, vertexShader);
	if (geometryShader != 0)
		glAttachShader(m_id, geometryShader);
	glAttachShader(m_id, fragmentShader);

	glLinkProgram(m_id);

	int linkStatus;
	glGetProgramiv(m_id, GL_LINK_STATUS, &linkStatus);
	if (!linkStatus)
	{
		char error[512];
		glGetProgramInfoLog(m_id, 512, NULL, error);

		std::string errorMessage("Failed to link shader : ");
		Log::error(errorMessage + error);
	}

	glDeleteShader(vertexShader);
	if (geometryShader != 0)
		glDeleteShader(geometryShader);
	glDeleteShader(fragmentShader);
}

DuckEngine::Shader::~Shader()
{
	glDeleteProgram(m_id);
}

void DuckEngine::Shader::setUniform(const std::string& name, bool value) const
{
	glUniform1i(glGetUniformLocation(m_id, name.c_str()), (int)value);
}

void DuckEngine::Shader::setUniform(const std::string& name, unsigned int value) const
{
	glUniform1ui(glGetUniformLocation(m_id, name.c_str()), value);
}

void DuckEngine::Shader::setUniform(const std::string& name, int value) const
{
	glUniform1i(glGetUniformLocation(m_id, name.c_str()), value);
}

void DuckEngine::Shader::setUniform(const std::string& name, float value) const
{
	glUniform1f(glGetUniformLocation(m_id, name.c_str()), value);
}

void DuckEngine::Shader::setUniform(const std::string& name, double value) const
{
	glUniform1d(glGetUniformLocation(m_id, name.c_str()), value);
}

void DuckEngine::Shader::setUniform(const std::string& name, const glm::vec3& value) const
{
	glUniform3f(glGetUniformLocation(m_id, name.c_str()), value.x, value.y, value.z);
}

void DuckEngine::Shader::setUniform(const std::string& name, const glm::mat4& value) const
{
	glUniformMatrix4fv(glGetUniformLocation(m_id, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
}

void DuckEngine::Shader::setUniform(const std::string& name, const std::vector<bool>& value) const
{
	std::vector<int> v;
	for (bool val : value)
		v.push_back((int)val);

	glUniform1iv(glGetUniformLocation(m_id, name.c_str()), (GLsizei)v.size(), v.data());
}

void DuckEngine::Shader::setUniform(const std::string& name, const std::vector<unsigned int>& value) const
{
	glUniform1uiv(glGetUniformLocation(m_id, name.c_str()), (GLsizei)value.size(), value.data());
}

void DuckEngine::Shader::setUniform(const std::string& name, const std::vector<int>& value) const
{
	glUniform1iv(glGetUniformLocation(m_id, name.c_str()), (GLsizei)value.size(), value.data());
}

void DuckEngine::Shader::setUniform(const std::string& name, const std::vector<float>& value) const
{
	glUniform1fv(glGetUniformLocation(m_id, name.c_str()), (GLsizei)value.size(), value.data());
}

void DuckEngine::Shader::setUniform(const std::string& name, const std::vector<double>& value) const
{
	glUniform1dv(glGetUniformLocation(m_id, name.c_str()), (GLsizei)value.size(), value.data());
}

void DuckEngine::Shader::setUniform(const std::string& name, const std::vector<glm::vec3>& value) const
{
	glUniform3fv(glGetUniformLocation(m_id, name.c_str()), (GLsizei)value.size(), glm::value_ptr(value[0]));
}

void DuckEngine::Shader::setUniform(const std::string& name, const std::vector<glm::mat4>& value) const
{
	glUniformMatrix4fv(glGetUniformLocation(m_id, name.c_str()), (GLsizei)value.size(), GL_FALSE, glm::value_ptr(value[0]));
}

void DuckEngine::Shader::use() const
{
	glUseProgram(m_id);
}