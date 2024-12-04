#include "qepch.h"
#include "OpenGLShader.h"

#include <fstream>
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

namespace QuasarEngine
{
	void OpenGLShader::bakeUniformLocations()
	{
		constexpr unsigned int MAX_UNIFORM_NAME_LENGTH = 64;

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
		}
	}

	unsigned int OpenGLShader::readShader(const std::string& source, unsigned int type)
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

	std::string OpenGLShader::readFile(const std::string& path)
	{
		std::ifstream file(path);
		if (!file.is_open())
			return "";

		std::ostringstream stream;
		stream << file.rdbuf();

		std::string str = stream.str();

		file.close();

		return str;
	}

	OpenGLShader::OpenGLShader(ShaderFile files) : m_id(0)
	{
		ShaderSource sources;

		if (!files.vertexShaderFile.empty())
		{
			sources.vertexShaderSource = readFile(files.vertexShaderFile);
		}

		if (!files.fragmentShaderFile.empty())
		{
			sources.fragmentShaderSource = readFile(files.fragmentShaderFile);
		}

		if (!files.geometryShaderFile.empty())
		{
			sources.geometryShaderSource = readFile(files.geometryShaderFile);
		}

		if (!files.computeShaderFile.empty())
		{
			sources.computeShaderSource = readFile(files.computeShaderFile);
		}

		if (!files.tessControlShaderFile.empty())
		{
			sources.tessControlShaderSource = readFile(files.tessControlShaderFile);
		}

		if (!files.tessEvaluationShaderFile.empty())
		{
			sources.tessEvaluationShaderSource = readFile(files.tessEvaluationShaderFile);
		}

		createShader(sources);
	}

	OpenGLShader::OpenGLShader(ShaderSource sources) : m_id(0)
	{
		createShader(sources);
	}

	OpenGLShader::~OpenGLShader()
	{
		glDeleteProgram(m_id);
	}

	void OpenGLShader::createShader(ShaderSource sources)
	{
		bool hasVertexShader = false;
		bool hasFragmentShader = false;
		bool hasGeometryShader = false;
		bool hasComputeShader = false;
		bool hasTessControlShader = false;
		bool hasTessEvaluationShader = false;

		unsigned int vertexShader;
		unsigned int fragmentShader;
		unsigned int geometryShader;
		unsigned int computeShader;
		unsigned int tessControlShader;
		unsigned int tessEvaluationShader;

		if (!sources.vertexShaderSource.empty())
		{
			hasVertexShader = true;
			vertexShader = readShader(sources.vertexShaderSource, GL_VERTEX_SHADER);
		}

		if (!sources.fragmentShaderSource.empty())
		{
			hasFragmentShader = true;
			fragmentShader = readShader(sources.fragmentShaderSource, GL_FRAGMENT_SHADER);
		}

		if (!sources.geometryShaderSource.empty())
		{
			hasGeometryShader = true;
			geometryShader = readShader(sources.geometryShaderSource, GL_GEOMETRY_SHADER);
		}

		if (!sources.computeShaderSource.empty())
		{
			hasComputeShader = true;
			computeShader = readShader(sources.computeShaderSource, GL_COMPUTE_SHADER);
		}

		if (!sources.tessControlShaderSource.empty())
		{
			hasTessControlShader = true;
			tessControlShader = readShader(sources.tessControlShaderSource, GL_TESS_CONTROL_SHADER);
		}

		if (!sources.tessEvaluationShaderSource.empty())
		{
			hasTessEvaluationShader = true;
			tessEvaluationShader = readShader(sources.tessEvaluationShaderSource, GL_TESS_EVALUATION_SHADER);
		}

		m_id = glCreateProgram();

		if (hasVertexShader)
			glAttachShader(m_id, vertexShader);

		if (hasFragmentShader)
			glAttachShader(m_id, fragmentShader);

		if (hasGeometryShader)
			glAttachShader(m_id, geometryShader);

		if (hasComputeShader)
			glAttachShader(m_id, computeShader);

		if (hasTessControlShader)
			glAttachShader(m_id, tessControlShader);

		if (hasTessEvaluationShader)
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

		if (hasVertexShader)
			glDeleteShader(vertexShader);

		if (hasFragmentShader)
			glDeleteShader(fragmentShader);

		if (hasGeometryShader)
			glDeleteShader(geometryShader);

		if (hasComputeShader)
			glDeleteShader(computeShader);

		if (hasTessControlShader)
			glDeleteShader(tessControlShader);

		if (hasTessEvaluationShader)
			glDeleteShader(tessEvaluationShader);

		bakeUniformLocations();
	}

	void OpenGLShader::setUniform(const std::string& name, bool value) const
	{
		glUniform1i(m_uniformLocations.at(name), (bool)value);
	}

	void OpenGLShader::setUniform(const std::string& name, uint32_t value) const
	{
		glUniform1ui(m_uniformLocations.at(name), value);
	}

	void OpenGLShader::setUniform(const std::string& name, int value) const
	{
		glUniform1i(m_uniformLocations.at(name), value);
	}

	void OpenGLShader::setUniform(const std::string& name, float value) const
	{
		glUniform1f(m_uniformLocations.at(name), value);
	}

	void OpenGLShader::setUniform(const std::string& name, double value) const
	{
		glUniform1d(m_uniformLocations.at(name), value);
	}

	void OpenGLShader::setUniform(const std::string& name, const glm::vec3& value) const
	{
		glUniform3f(m_uniformLocations.at(name), value.x, value.y, value.z);
	}

	void OpenGLShader::setUniform(const std::string& name, const glm::mat3& value) const
	{
		glUniformMatrix3fv(m_uniformLocations.at(name), 1, GL_FALSE, glm::value_ptr(value));
	}

	void OpenGLShader::setUniform(const std::string& name, const glm::mat4& value) const
	{
		glUniformMatrix4fv(m_uniformLocations.at(name), 1, GL_FALSE, glm::value_ptr(value));
	}

	void OpenGLShader::setUniform(const std::string& name, const std::vector<bool>& value) const
	{
		std::vector<int> v;
		for (bool val : value)
			v.push_back((int)val);

		glUniform1iv(m_uniformLocations.at(name), (GLsizei)v.size(), v.data());
	}

	void OpenGLShader::setUniform(const std::string& name, const std::vector<unsigned int>& value) const
	{
		glUniform1uiv(m_uniformLocations.at(name), (GLsizei)value.size(), value.data());
	}

	void OpenGLShader::setUniform(const std::string& name, const std::vector<int>& value) const
	{
		glUniform1iv(m_uniformLocations.at(name), (GLsizei)value.size(), value.data());
	}

	void OpenGLShader::setUniform(const std::string& name, const std::vector<float>& value) const
	{
		glUniform1fv(m_uniformLocations.at(name), (GLsizei)value.size(), value.data());
	}

	void OpenGLShader::setUniform(const std::string& name, const std::vector<double>& value) const
	{
		glUniform1dv(m_uniformLocations.at(name), (GLsizei)value.size(), value.data());
	}

	void OpenGLShader::setUniform(const std::string& name, const std::vector<glm::vec3>& value) const
	{
		glUniform3fv(m_uniformLocations.at(name), (GLsizei)value.size(), glm::value_ptr(value[0]));
	}

	void OpenGLShader::setUniform(const std::string& name, const std::vector<glm::mat4>& value) const
	{
		glUniformMatrix4fv(m_uniformLocations.at(name), (GLsizei)value.size(), GL_FALSE, glm::value_ptr(value[0]));
	}

	void OpenGLShader::use() const
	{
		glUseProgram(m_id);
	}

	void OpenGLShader::unuse() const
	{
		glUseProgram(0);
	}
}