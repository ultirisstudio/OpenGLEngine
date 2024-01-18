#pragma once

#include <OpenGLEngine/Entity/Component.h>

class Entity;

namespace OpenGLEngine
{
	class ModelComponent : public Component
	{
	private:
		std::shared_ptr<Model> m_Model;

		std::vector<Entity> m_SubEntities;
	public:
		std::string m_ModelPath;

		ModelComponent() = default;
		ModelComponent(const std::string& path);

		void SetModel(const std::string& path);

		std::vector<Entity>* GetSubEntities() { return &m_SubEntities; }

		Model& GetModel() { return *m_Model; }
		Model* GetPtr() { return m_Model.get(); }
	};
}