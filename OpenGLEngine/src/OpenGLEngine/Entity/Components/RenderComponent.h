#pragma once

#include <OpenGLEngine/Entity/Component.h>
#include <OpenGLEngine/Shader/Shader.h>

class RenderComponent : public Component
{
private:
	OpenGLEngine::Shader m_Shader;
public:
	bool m_CanDraw;

	RenderComponent();

	void Draw(bool runtime);
	void GenerateShader();

	OpenGLEngine::Shader& GetShader() { return m_Shader; }
};