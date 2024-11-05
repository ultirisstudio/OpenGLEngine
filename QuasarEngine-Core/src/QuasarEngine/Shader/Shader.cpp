#include "qepch.h"

#include <QuasarEngine/Shader/Shader.h>
#include "QuasarEngine/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLShader.h"

namespace QuasarEngine
{
	std::shared_ptr<Shader> Shader::Create(ShaderFile files)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:    return nullptr;
		case RendererAPI::API::OpenGL:  return std::make_shared<OpenGLShader>(files);
		}

		return nullptr;
	}

	std::shared_ptr<Shader> Shader::Create(ShaderSource sources)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:    return nullptr;
		case RendererAPI::API::OpenGL:  return std::make_shared<OpenGLShader>(sources);
		}

		return nullptr;
	}
}