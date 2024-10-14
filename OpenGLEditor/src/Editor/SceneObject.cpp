#include "SceneObject.h"

namespace OpenGLEngine
{
	SceneObject::SceneObject()
	{
		//m_Scene = std::make_unique<Scene>();
	}

	void SceneObject::CreateScene()
	{
		//m_Scene = std::make_unique<Scene>();
	}

	/*Entity SceneObject::GetPrimaryCameraEntity()
	{
		auto view = m_Scene->GetAllEntitiesWith<CameraComponent>();
		for (auto entity : view)
		{
			auto& cameraComponent = view.get<CameraComponent>(entity);
			if (cameraComponent.Primary)
				return Entity{ entity, m_Scene->GetRegistry() };
		}
		return entt::null;
	}*/

	/*Camera& SceneObject::GetPrimaryCamera()
	{
		return GetPrimaryCameraEntity().GetComponent<CameraComponent>().GetCamera();
	}

	bool SceneObject::HasPrimaryCamera()
	{
		return GetPrimaryCameraEntity() != entt::null;
	}*/
}