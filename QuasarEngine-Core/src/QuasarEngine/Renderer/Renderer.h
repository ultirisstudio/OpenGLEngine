#pragma once

#include <QuasarEngine/Resources/Texture.h>
#include <QuasarEngine/Resources/Model.h>
#include <QuasarEngine/Resources/Materials/Material.h>
#include <QuasarEngine/Shader/Shader.h>
#include <QuasarEngine/Resources/ResourceManager.h>
#include <QuasarEngine/Scene/Scene.h>
#include <QuasarEngine/Scene/BaseCamera.h>

#include <filesystem>

namespace QuasarEngine
{
	class Renderer
	{
	public:
		struct SceneData
		{
			Scene* m_Scene;
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