#pragma once

#include <QuasarEngine/Resources/ResourceManager.h>
#include <QuasarEngine/Shader/Shader.h>
#include <QuasarEngine/Scene/Scene.h>
#include <QuasarEngine/Scene/BaseCamera.h>
#include <QuasarEngine/Asset/AssetRegistry.h>
#include <QuasarEngine/Asset/AssetManager.h>

#include <QuasarEngine/Renderer/RenderCommand.h>

#include <filesystem>

namespace QuasarEngine
{
	class Renderer
	{
	public:
		struct SceneData
		{
			Scene* m_Scene;
			//std::unique_ptr<ResourceManager> m_ResourceManager;
			//std::unique_ptr<AssetRegistry> m_AssetRegistry;
			std::unique_ptr<AssetManager> m_AssetManager;

			std::shared_ptr<Shader> m_Shader;
		};
		static SceneData m_SceneData;

		struct DebugRenderData
		{
			std::shared_ptr<Shader> m_DebugLineShader;
			std::shared_ptr<Shader> m_DebugTriangleShader;

			const glm::vec3& GetColorFromUint_32t(uint32_t color);
		};
		static DebugRenderData m_DebugRenderData;

		static void Init();

		static void BeginScene(Scene& scene);
		static void Render(BaseCamera& camera);
		static void RenderSkybox(BaseCamera& camera);
		static void EndScene();

		static void LoadModel(const std::string& path);

		static Scene* GetScene();

		static double GetTime();

		static RendererAPI::API GetAPI();
	};
}