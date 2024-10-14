#pragma once

#include "../SceneObject.h"

#include <OpenGLEngine/ECS/Entity.h>

namespace OpenGLEngine
{
	class SceneHierarchy
	{
	public:
		SceneHierarchy();

		void OnImGuiRender(SceneObject& sceneObject);

		Entity m_SelectedEntity;
	private:
		void OnDrawEntityNode(SceneObject& sceneObject, Entity entity);
	};
}