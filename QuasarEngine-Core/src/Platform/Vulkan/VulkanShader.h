#pragma once

#include <string>
#include <unordered_map>

#include <QuasarEngine/Shader/Shader.h>

namespace QuasarEngine
{
	class VulkanShader : public Shader
	{
	public:
		VulkanShader(ShaderFile files);
		VulkanShader(ShaderSource sources);
		~VulkanShader();

		void createShader(ShaderSource sources);

		void setUniform(const std::string& name, bool value) const override;
		void setUniform(const std::string& name, uint32_t value) const override;
		void setUniform(const std::string& name, int value) const override;
		void setUniform(const std::string& name, float value) const override;
		void setUniform(const std::string& name, double value) const override;
		void setUniform(const std::string& name, const glm::vec3& value) const override;
		void setUniform(const std::string& name, const glm::mat3& value) const override;
		void setUniform(const std::string& name, const glm::mat4& value) const override;

		void setUniform(const std::string& name, const std::vector<bool>& value) const override;
		void setUniform(const std::string& name, const std::vector<unsigned int>& value) const override;
		void setUniform(const std::string& name, const std::vector<int>& value) const override;
		void setUniform(const std::string& name, const std::vector<float>& value) const override;
		void setUniform(const std::string& name, const std::vector<double>& value) const override;
		void setUniform(const std::string& name, const std::vector<glm::vec3>& value) const override;
		void setUniform(const std::string& name, const std::vector<glm::mat4>& value) const override;

		void use() const override;
		void unuse() const override;
	};
}