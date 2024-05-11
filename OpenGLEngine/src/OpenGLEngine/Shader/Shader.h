#pragma once

#include <glm/glm.hpp>
#include <vector>

namespace OpenGLEngine
{
	class Shader
	{
	private:
		unsigned int m_id;

		unsigned int createShader(const std::string& source, unsigned int type);
		unsigned int ReadShader(const std::string& path, unsigned int type);

	public:
		Shader();
		~Shader();

		void LoadFromFile(const std::string& path_vs, const std::string& path_fs);
		void LoadFromFile(const std::string& path_vs, const std::string& path_fs, const std::string& path_tcs, const std::string& path_tes);

		void setUniform(const std::string& name, bool value) const;
		//void setUniform(const std::string& name, unsigned int value) const;
		void setUniform(const std::string& name, uint32_t value) const;
		void setUniform(const std::string& name, int value) const;
		void setUniform(const std::string& name, float value) const;
		void setUniform(const std::string& name, double value) const;
		void setUniform(const std::string& name, const glm::vec3& value) const;
		void setUniform(const std::string& name, const glm::mat3& value) const;
		void setUniform(const std::string& name, const glm::mat4& value) const;

		void setUniform(const std::string& name, const std::vector<bool>& value) const;
		void setUniform(const std::string& name, const std::vector<unsigned int>& value) const;
		void setUniform(const std::string& name, const std::vector<int>& value) const;
		void setUniform(const std::string& name, const std::vector<float>& value) const;
		void setUniform(const std::string& name, const std::vector<double>& value) const;
		void setUniform(const std::string& name, const std::vector<glm::vec3>& value) const;
		void setUniform(const std::string& name, const std::vector<glm::mat4>& value) const;

		void use() const;
	};
}