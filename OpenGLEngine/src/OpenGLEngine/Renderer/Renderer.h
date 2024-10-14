#pragma once

#include <OpenGLEngine/Resources/Texture.h>
#include <OpenGLEngine/Resources/Model.h>
#include <OpenGLEngine/Resources/Materials/Material.h>
#include <OpenGLEngine/Shader/Shader.h>
#include <OpenGLEngine/Resources/ResourceManager.h>
#include <OpenGLEngine/Scene/Scene.h>
#include <OpenGLEngine/Scene/BaseCamera.h>
#include <OpenGLEngine/Scene/Skybox.h>

#include <filesystem>

namespace OpenGLEngine
{
	class Renderer
	{
	public:
		struct SceneData
		{
			Scene* m_Scene;
			Skybox* m_Skybox;
			ResourceManager m_ResourceManager;
			Shader m_Shader;
		};
		static SceneData m_SceneData;

		struct DebugRenderData
		{
			Shader m_DebugLineShader;
			Shader m_DebugTriangleShader;

			const glm::vec3& GetColorFromUint_32t(uint32_t color);
		};
		static DebugRenderData m_DebugRenderData;

		static void Init();

		static void BeginScene(Scene& scene);
		static void Render(BaseCamera& camera);
		static void RenderSkybox(BaseCamera& camera);
		static void EndScene();

		static void LoadModel(const std::string& path);

		static void Clear();
		static void ClearColor(const glm::vec4& color);
		static void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height);

		static Scene* GetScene();

		static double GetTime();
	};
}