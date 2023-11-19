#pragma once

#include <OpenGLEngine/Entity/Entity.h>
#include <OpenGLEngine/Scene/Scene.h>

namespace OpenGLEngine
{
	class SceneHierarchy
	{
	public:
		SceneHierarchy();

		void OnImGuiRender(Scene& scene);
	private:
	};
}