#include "qepch.h"

#include <QuasarEngine/Shader/Shader.h>
#include <glm/gtc/type_ptr.hpp>

#include <fstream>
#include <sstream>

namespace QuasarEngine
{
	/*unsigned int Shader::createShader(const std::string& source, unsigned int type)
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
			std::cout << errorMessage << error << std::endl;

			return 0;
		}

		return shader;
	}*/

	unsigned int Shader::ReadShader(const std::string& path, unsigned int type)
	{
		/*std::ifstream file(path);
		if (!file.is_open())
			return 0;

		std::ostringstream stream;
		stream << file.rdbuf();

		std::string str = stream.str();
		const char* source = str.c_str();

		file.close();
		*/
		unsigned int shader;
		/*
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
		}*/

		return shader;
	}

	Shader::Shader() : m_id(0)
	{

	}

	Shader::~Shader()
	{
		//glDeleteProgram(m_id);
	}

	void Shader::bakeUniformLocations()
	{
		/*constexpr unsigned int MAX_UNIFORM_NAME_LENGTH = 64;

		if (m_id == 0)
			return;

		int activeUniforms = 0;
		glGetProgramiv(m_id, GL_ACTIVE_UNIFORMS, &activeUniforms);

		for (int i = 0; i < activeUniforms; ++i)
		{
			std::string name(MAX_UNIFORM_NAME_LENGTH, '\0');
			unsigned int type = 0;
			int size = 0;
			int length = 0;

			glGetActiveUniform(m_id, static_cast<unsigned int>(i), MAX_UNIFORM_NAME_LENGTH, &length, &size, &type, name.data());

			std::string uniformName = name.substr(0, name.find('\0'));
			unsigned int uniformLocation = glGetUniformLocation(m_id, uniformName.c_str());

			m_uniformLocations.insert({ uniformName, uniformLocation });
		}*/
	}

	void Shader::LoadFromFile(const std::string& path_vs, const std::string& path_fs)
	{
		/*unsigned int vertexShader = ReadShader(path_vs, GL_VERTEX_SHADER);
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

		bakeUniformLocations();*/
	}

	void Shader::LoadFromFile(const std::string& path_vs, const std::string& path_fs, const std::string& path_tcs, const std::string& path_tes)
	{
		/*unsigned int vertexShader = ReadShader(path_vs, GL_VERTEX_SHADER);
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

		bakeUniformLocations();*/
	}

	void Shader::setUniform(const std::string& name, bool value) const
	{
		//glUniform1i(m_uniformLocations.at(name), (bool)value);
	}

	void Shader::setUniform(const std::string& name, uint32_t value) const
	{
		//glUniform1ui(m_uniformLocations.at(name), value);
	}

	/*void Shader::setUniform(const std::string& name, unsigned int value) const
	{
		glUniform1ui(m_uniformLocations.at(name), value);
	}*/

	void Shader::setUniform(const std::string& name, int value) const
	{
		//glUniform1i(m_uniformLocations.at(name), value);
	}

	void Shader::setUniform(const std::string& name, float value) const
	{
		//glUniform1f(m_uniformLocations.at(name), value);
	}

	void Shader::setUniform(const std::string& name, double value) const
	{
		//glUniform1d(m_uniformLocations.at(name), value);
	}

	void Shader::setUniform(const std::string& name, const glm::vec3& value) const
	{
		//glUniform3f(m_uniformLocations.at(name), value.x, value.y, value.z);
	}

	void Shader::setUniform(const std::string& name, const glm::mat3& value) const
	{
		//glUniformMatrix3fv(m_uniformLocations.at(name), 1, GL_FALSE, glm::value_ptr(value));
	}

	void Shader::setUniform(const std::string& name, const glm::mat4& value) const
	{
		//glUniformMatrix4fv(m_uniformLocations.at(name), 1, GL_FALSE, glm::value_ptr(value));
	}

	void Shader::setUniform(const std::string& name, const std::vector<bool>& value) const
	{
		std::vector<int> v;
		for (bool val : value)
			v.push_back((int)val);

		//glUniform1iv(m_uniformLocations.at(name), (GLsizei)v.size(), v.data());
	}

	void Shader::setUniform(const std::string& name, const std::vector<unsigned int>& value) const
	{
		//glUniform1uiv(m_uniformLocations.at(name), (GLsizei)value.size(), value.data());
	}

	void Shader::setUniform(const std::string& name, const std::vector<int>& value) const
	{
		//glUniform1iv(m_uniformLocations.at(name), (GLsizei)value.size(), value.data());
	}

	void Shader::setUniform(const std::string& name, const std::vector<float>& value) const
	{
		//glUniform1fv(m_uniformLocations.at(name), (GLsizei)value.size(), value.data());
	}

	void Shader::setUniform(const std::string& name, const std::vector<double>& value) const
	{
		//glUniform1dv(m_uniformLocations.at(name), (GLsizei)value.size(), value.data());
	}

	void Shader::setUniform(const std::string& name, const std::vector<glm::vec3>& value) const
	{
		//glUniform3fv(m_uniformLocations.at(name), (GLsizei)value.size(), glm::value_ptr(value[0]));
	}

	void Shader::setUniform(const std::string& name, const std::vector<glm::mat4>& value) const
	{
		//glUniformMatrix4fv(m_uniformLocations.at(name), (GLsizei)value.size(), GL_FALSE, glm::value_ptr(value[0]));
	}

	void Shader::use() const
	{
		//glUseProgram(m_id);
	}
}