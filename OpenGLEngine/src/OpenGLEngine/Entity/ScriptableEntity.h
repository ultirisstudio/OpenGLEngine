#pragma once

#include "Entity.h"

namespace OpenGLEngine
{
	class ScriptableEntity
	{
	public:
		virtual ~ScriptableEntity() {}

		template<typename T>
		T& GetComponent() {
			return m_Entity.GetComponent<T>();
		}

	public:
		virtual void OnCreate() {}
		virtual void OnDestroy() {}
		virtual void OnUpdate(double deltaTime) {}
	private:
		Entity m_Entity;
		friend class Scene;
	};
}