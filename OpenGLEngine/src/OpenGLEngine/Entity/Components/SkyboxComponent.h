#pragma once

#include <OpenGLEngine/Entity/Component.h>

class SkyboxComponent : public Component
{
private:
	std::shared_ptr<OpenGLEngine::Model> m_Model;
	OpenGLEngine::CubeMap m_CubeMap;
	OpenGLEngine::Shader m_CubeMapShader;
public:
	SkyboxComponent(const std::string& path)
	{
		m_CubeMapShader.LoadFromFile("Shaders/cubemap.vert", "Shaders/cubemap.frag");
		m_Model = OpenGLEngine::Model::CreateModel(path);
		m_CubeMap.Load({ "Assets/Skybox/right.jpg", "Assets/Skybox/left.jpg", "Assets/Skybox/top.jpg", "Assets/Skybox/bottom.jpg", "Assets/Skybox/front.jpg", "Assets/Skybox/back.jpg" });
	}

	OpenGLEngine::CubeMap* GetCubeMap()
	{
		return &m_CubeMap;
	}

	OpenGLEngine::Shader* GetCubeMapShader()
	{
		return &m_CubeMapShader;
	}

	OpenGLEngine::Model* GetModel()
	{
		return m_Model.get();
	}
};