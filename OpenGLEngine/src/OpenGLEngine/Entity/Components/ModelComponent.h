#pragma once

#include <OpenGLEngine/Entity/Component.h>

class Mesh;

namespace OpenGLEngine
{
	class ModelComponent : public Component
	{
	private:
		std::shared_ptr<Model> m_Model;

		std::vector<Entity*> m_SubEntities;
	public:
		std::string m_ModelPath;

		ModelComponent() = default;
		ModelComponent(const std::string& path);

		void SetModel(const std::string& path);

		std::unordered_map<std::string, Mesh*>& GetMeshes() { return m_Model->GetMeshes(); }

		Model& GetModel() { return *m_Model; }
		Model* GetPtr() { return m_Model.get(); }
	};
}