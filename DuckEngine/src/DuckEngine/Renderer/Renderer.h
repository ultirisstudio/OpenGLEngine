#pragma once

#include <DuckEngine/Renderer/RenderModel.h>
#include <DuckEngine/Renderer/Camera.h>
#include <DuckEngine/Shader/ShaderGenerator.h>

namespace DuckEngine
{
	class Renderer
	{
	public:
		struct SceneData
		{
			glm::mat4 m_viewMatrix;
			glm::mat4 m_projectionMatrix;

			ShaderGenerator m_shaderGenerator;
		};
		static SceneData m_SceneData;

		static Shader* CreateShader(Material& material);

		static const glm::mat4& getViewMatrix();
		static const glm::mat4& getProjectionMatrix();

		static void BeginScene(Camera* camera);
		static void UpdateMatrix(glm::mat4 viewMatrix,glm::mat4 projectionMatrix);
		static void EndScene();

		static std::shared_ptr<RenderModel> CreateRenderModel(const std::string& path);

		static void Clear();
		static void ClearColor(const glm::vec4& color);
		static void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height);
	};
}