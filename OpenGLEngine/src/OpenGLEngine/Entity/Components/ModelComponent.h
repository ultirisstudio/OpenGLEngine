#pragma once

#include <OpenGLEngine/Entity/Component.h>

class ModelComponent : public Component
{
private:
	std::shared_ptr<OpenGLEngine::Model> m_Model;
public:
	std::string m_ModelPath;

	ModelComponent() = default;
	ModelComponent(const std::string& path);

	void SetModel(const std::string& path);

	OpenGLEngine::Model& GetModel() { return *m_Model; }
	OpenGLEngine::Model* GetPtr() { return m_Model.get(); }
};