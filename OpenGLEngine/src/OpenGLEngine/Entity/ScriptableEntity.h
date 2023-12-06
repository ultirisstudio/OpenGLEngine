#pragma once

#include "Entity.h"

#include <GLFW/glfw3.h>
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

		bool GetKeyPressed(KeyCode key)
		{
			auto* window = m_Window;
			if (window)
			{
				auto state = glfwGetKey(window, static_cast<int32_t>(key));
				std::cout << state << std::endl;
				return state == GLFW_PRESS;
			}
			return false;
		}

	public:
		virtual void OnCreate() {}
		virtual void OnDestroy() {}
		virtual void OnUpdate(double deltaTime) {}

		Entity m_Entity;
		GLFWwindow* m_Window = nullptr;
	private:
		friend class Scene;
	};
}