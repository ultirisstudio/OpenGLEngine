#pragma once

#include <glm/glm.hpp>
#include <vector>
#include <string>

namespace QuasarEngine
{
	struct ShaderFile
	{
		std::string vertexShaderFile = "";
		std::string fragmentShaderFile = "";
		std::string geometryShaderFile = "";
		std::string computeShaderFile = "";
		std::string tessControlShaderFile = "";
		std::string tessEvaluationShaderFile = "";
	};

	struct ShaderSource
	{
		std::string vertexShaderSource = "";
		std::string fragmentShaderSource = "";
		std::string geometryShaderSource = "";
		std::string computeShaderSource = "";
		std::string tessControlShaderSource = "";
		std::string tessEvaluationShaderSource = "";
	};

	class Shader
	{
	public:
		virtual ~Shader() = default;

		static std::shared_ptr<Shader> Create(ShaderFile files);
		static std::shared_ptr<Shader> Create(ShaderSource sources);

		virtual void setUniform(const std::string& name, bool value) const = 0;
		virtual void setUniform(const std::string& name, uint32_t value) const = 0;
		virtual void setUniform(const std::string& name, int value) const = 0;
		virtual void setUniform(const std::string& name, float value) const = 0;
		virtual void setUniform(const std::string& name, double value) const = 0;
		virtual void setUniform(const std::string& name, const glm::vec3& value) const = 0;
		virtual void setUniform(const std::string& name, const glm::mat3& value) const = 0;
		virtual void setUniform(const std::string& name, const glm::mat4& value) const = 0;

		virtual void setUniform(const std::string& name, const std::vector<bool>& value) const = 0;
		virtual void setUniform(const std::string& name, const std::vector<unsigned int>& value) const = 0;
		virtual void setUniform(const std::string& name, const std::vector<int>& value) const = 0;
		virtual void setUniform(const std::string& name, const std::vector<float>& value) const = 0;
		virtual void setUniform(const std::string& name, const std::vector<double>& value) const = 0;
		virtual void setUniform(const std::string& name, const std::vector<glm::vec3>& value) const = 0;
		virtual void setUniform(const std::string& name, const std::vector<glm::mat4>& value) const = 0;

		virtual void use() const = 0;
	};
}