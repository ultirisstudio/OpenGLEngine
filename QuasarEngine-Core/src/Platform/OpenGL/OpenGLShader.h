#pragma once

#include <string>
#include <unordered_map>

#include <QuasarEngine/Shader/Shader.h>

namespace QuasarEngine
{
	class OpenGLShader : public Shader
	{
	private:
		unsigned int m_id;

		std::unordered_map<std::string, int> m_uniformLocations;

		void bakeUniformLocations();
		std::string readFile(const std::string& path);
		unsigned int readShader(const std::string& source, unsigned int type);
	public:
		OpenGLShader(ShaderFile files);
		OpenGLShader(ShaderSource sources);
		~OpenGLShader();

		void createShader(ShaderSource sources);

		virtual void setUniform(const std::string& name, bool value) const override;
		virtual void setUniform(const std::string& name, uint32_t value) const override;
		virtual void setUniform(const std::string& name, int value) const override;
		virtual void setUniform(const std::string& name, float value) const override;
		virtual void setUniform(const std::string& name, double value) const override;
		virtual void setUniform(const std::string& name, const glm::vec3& value) const override;
		virtual void setUniform(const std::string& name, const glm::mat3& value) const override;
		virtual void setUniform(const std::string& name, const glm::mat4& value) const override;

		virtual void setUniform(const std::string& name, const std::vector<bool>& value) const override;
		virtual void setUniform(const std::string& name, const std::vector<unsigned int>& value) const override;
		virtual void setUniform(const std::string& name, const std::vector<int>& value) const override;
		virtual void setUniform(const std::string& name, const std::vector<float>& value) const override;
		virtual void setUniform(const std::string& name, const std::vector<double>& value) const override;
		virtual void setUniform(const std::string& name, const std::vector<glm::vec3>& value) const override;
		virtual void setUniform(const std::string& name, const std::vector<glm::mat4>& value) const override;

		virtual void use() const;
	};
}