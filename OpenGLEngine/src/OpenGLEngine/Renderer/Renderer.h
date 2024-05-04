#pragma once

#include <OpenGLEngine/Scene/EditorCamera.h>
#include <OpenGLEngine/Resources/Texture.h>
#include <OpenGLEngine/Resources/Model.h>
#include <OpenGLEngine/Resources/Materials/Material.h>
#include <OpenGLEngine/Shader/Shader.h>
#include <OpenGLEngine/Resources/ResourceManager.h>
#include <OpenGLEngine/Scene/Scene.h>
#include <OpenGLEngine/Scene/BaseCamera.h>

namespace OpenGLEngine
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

		static void Init();

		static void BeginScene(Scene& scene);
		static void Render(BaseCamera& camera);
		static void EndScene();

		static void Clear();
		static void ClearColor(const glm::vec4& color);
		static void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height);

		static Scene* GetScene();

		static double GetTime();
	};
}