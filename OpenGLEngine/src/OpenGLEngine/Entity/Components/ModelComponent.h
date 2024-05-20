#pragma once

#include <OpenGLEngine/Entity/Component.h>
#include <OpenGLEngine/Resources/Model.h>

namespace OpenGLEngine
{
	class ModelComponent : public Component
	{
	private:
		std::shared_ptr<Model> m_Model;
	public:
		std::string m_ModelPath;

		ModelComponent();

		void SetModel(const std::string& path);

		Model& GetModel() { return *m_Model; }
		Model* GetPtr() { return m_Model.get(); }
	};
}