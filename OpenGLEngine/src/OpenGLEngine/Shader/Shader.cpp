#include "depch.h"
#include <OpenGLEngine/Shader/Shader.h>
#include <OpenGLEngine/Tools/Log.h>
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>
#include <fstream>
#include <sstream>

namespace OpenGLEngine
{
	unsigned int Shader::createShader(const std::string& source, unsigned int type)
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

	unsigned int Shader::ReadShader(const std::string& path, unsigned int type)
	{
		std::ifstream file(path);
		if (!file.is_open())
			return 0;

		std::ostringstream stream;
		stream << file.rdbuf();

		std::string str = stream.str();
		const char* source = str.c_str();

		file.close();

		unsigned int shader;
		shader = glCreateShader(type);

		glShaderSource(shader, 1, &source, NULL);
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
			case GL_GEOMETRY_SHADER:
				shaderType = "geometry";
				break;
			case GL_FRAGMENT_SHADER:
				shaderType = "fragment";
				break;
			default:
				break;
			}
			std::cout << "Failed to compile " << shaderType << " shader : " << error << std::endl;
			return 0;
		}

		return shader;
	}

	Shader::Shader() : m_id(0)
	{

	}

	Shader::~Shader()
	{
		glDeleteProgram(m_id);
	}

	void Shader::LoadFromFile(const std::string& path_vs, const std::string& path_fs)
	{
		unsigned int vertexShader = ReadShader(path_vs, GL_VERTEX_SHADER);
		unsigned int fragmentShader = ReadShader(path_fs, GL_FRAGMENT_SHADER);

		m_id = glCreateProgram();

		glAttachShader(m_id, vertexShader);
		glAttachShader(m_id, fragmentShader);

		glLinkProgram(m_id);

		int linkStatus;
		glGetProgramiv(m_id, GL_LINK_STATUS, &linkStatus);
		if (!linkStatus)
		{
			char error[512];
			glGetProgramInfoLog(m_id, 512, NULL, error);
			std::cout << "Failed to link shader : " << error << std::endl;
		}

		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
	}

	void Shader::LoadFromFile(const std::string& path_vs, const std::string& path_fs, const std::string& path_tcs, const std::string& path_tes)
	{
		unsigned int vertexShader = ReadShader(path_vs, GL_VERTEX_SHADER);
		unsigned int fragmentShader = ReadShader(path_fs, GL_FRAGMENT_SHADER);
		unsigned int tessControlShader = ReadShader(path_tcs, GL_TESS_CONTROL_SHADER);
		unsigned int tessEvaluationShader = ReadShader(path_tes, GL_TESS_EVALUATION_SHADER);

		m_id = glCreateProgram();

		glAttachShader(m_id, vertexShader);
		glAttachShader(m_id, fragmentShader);
		glAttachShader(m_id, tessControlShader);
		glAttachShader(m_id, tessEvaluationShader);

		glLinkProgram(m_id);

		int linkStatus;
		glGetProgramiv(m_id, GL_LINK_STATUS, &linkStatus);
		if (!linkStatus)
		{
			char error[512];
			glGetProgramInfoLog(m_id, 512, NULL, error);
			std::cout << "Failed to link shader : " << error << std::endl;
		}

		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
		glDeleteShader(tessControlShader);
		glDeleteShader(tessEvaluationShader);
	}

	void Shader::setUniform(const std::string& name, bool value) const
	{
		glUniform1i(glGetUniformLocation(m_id, name.c_str()), (bool)value);
	}

	void Shader::setUniform(const std::string& name, unsigned int value) const
	{
		glUniform1ui(glGetUniformLocation(m_id, name.c_str()), value);
	}

	void Shader::setUniform(const std::string& name, int value) const
	{
		glUniform1i(glGetUniformLocation(m_id, name.c_str()), value);
	}

	void Shader::setUniform(const std::string& name, float value) const
	{
		glUniform1f(glGetUniformLocation(m_id, name.c_str()), value);
	}

	void Shader::setUniform(const std::string& name, double value) const
	{
		glUniform1d(glGetUniformLocation(m_id, name.c_str()), value);
	}

	void Shader::setUniform(const std::string& name, const glm::vec3& value) const
	{
		glUniform3f(glGetUniformLocation(m_id, name.c_str()), value.x, value.y, value.z);
	}

	void Shader::setUniform(const std::string& name, const glm::mat3& value) const
	{
		glUniformMatrix3fv(glGetUniformLocation(m_id, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
	}

	void Shader::setUniform(const std::string& name, const glm::mat4& value) const
	{
		glUniformMatrix4fv(glGetUniformLocation(m_id, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
	}

	void Shader::setUniform(const std::string& name, const std::vector<bool>& value) const
	{
		std::vector<int> v;
		for (bool val : value)
			v.push_back((int)val);

		glUniform1iv(glGetUniformLocation(m_id, name.c_str()), (GLsizei)v.size(), v.data());
	}

	void Shader::setUniform(const std::string& name, const std::vector<unsigned int>& value) const
	{
		glUniform1uiv(glGetUniformLocation(m_id, name.c_str()), (GLsizei)value.size(), value.data());
	}

	void Shader::setUniform(const std::string& name, const std::vector<int>& value) const
	{
		glUniform1iv(glGetUniformLocation(m_id, name.c_str()), (GLsizei)value.size(), value.data());
	}

	void Shader::setUniform(const std::string& name, const std::vector<float>& value) const
	{
		glUniform1fv(glGetUniformLocation(m_id, name.c_str()), (GLsizei)value.size(), value.data());
	}

	void Shader::setUniform(const std::string& name, const std::vector<double>& value) const
	{
		glUniform1dv(glGetUniformLocation(m_id, name.c_str()), (GLsizei)value.size(), value.data());
	}

	void Shader::setUniform(const std::string& name, const std::vector<glm::vec3>& value) const
	{
		glUniform3fv(glGetUniformLocation(m_id, name.c_str()), (GLsizei)value.size(), glm::value_ptr(value[0]));
	}

	void Shader::setUniform(const std::string& name, const std::vector<glm::mat4>& value) const
	{
		glUniformMatrix4fv(glGetUniformLocation(m_id, name.c_str()), (GLsizei)value.size(), GL_FALSE, glm::value_ptr(value[0]));
	}

	void Shader::use() const
	{
		glUseProgram(m_id);
	}
}