#pragma once

#include "Entity.h"

#include <OpenGLEngine/Core/KeyCodes.h>

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

		template<typename T>
		bool HasComponent() {
			return m_Entity.HasComponent<T>();
		}

		template<typename T>
		void RemoveComponent() {
			m_Entity.RemoveComponent<T>();
		}

		template<typename T, typename... TArgs>
		T& AddComponent(TArgs&&... mArgs) {
			return m_Entity.AddComponent<T>(std::forward<TArgs>(mArgs)...);
		}

		bool GetKeyPressed(KeyCode key);

	public:
		virtual void OnCreate() {}
		virtual void OnDestroy() {}
		virtual void OnUpdate(double deltaTime) {}

		Entity m_Entity;
	private:
		friend class Scene;
	};
}