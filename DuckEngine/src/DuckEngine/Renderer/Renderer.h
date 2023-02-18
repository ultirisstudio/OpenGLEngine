#pragma once

#include <DuckEngine/Scene/Camera.h>
#include <DuckEngine/Resources/Texture.h>
#include <DuckEngine/Resources/Model.h>
#include <DuckEngine/Resources/Materials/Material.h>
#include <DuckEngine/Shader/Shader.h>
#include <DuckEngine/Resources/ResourceManager.h>

namespace DuckEngine
{
	class Renderer
	{
	public:
		struct SceneData
		{
			Camera* m_ActiveCamera;
			ResourceManager m_ResourceManager;
		};
		static SceneData m_SceneData;

		static void BeginScene(Camera* camera);
		static void EndScene();

		static void Clear();
		static void ClearColor(const glm::vec4& color);
		static void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height);
	};
}