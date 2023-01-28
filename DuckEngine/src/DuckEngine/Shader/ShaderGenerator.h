#pragma once

#include <DuckEngine/Renderer/RenderModel.h>
#include <DuckEngine/Shader/Shader.h>

namespace DuckEngine
{
	class ShaderGenerator
	{
	private:
		std::map<std::string, Shader*> m_shaders;

		ShaderGenerator();
		~ShaderGenerator();

	public:
		Shader* generateShader(Material& material);
		void generateShader(const std::string& id, Material& material);
		void freeShader(const std::string& id);
		Shader& getShader(const std::string& id);

		friend class Renderer;
	};
}