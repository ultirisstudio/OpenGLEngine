#pragma once

#include <filesystem>

#include <OpenGLEngine/Scene/Camera.h>
#include <OpenGLEngine/ECS/Components/CameraComponent.h>
//#include <OpenGLEngine/Scene/Scene.h>
//#include <OpenGLEngine/ECS/Entity.h>

namespace OpenGLEngine
{
	class SceneObject
	{
	private:
		//std::unique_ptr<Scene> m_Scene;
		//std::unique_ptr<Camera> m_Camera;

		std::string m_Name;
		std::string m_Path;
	public:
		SceneObject();

		//Entity GetPrimaryCameraEntity();
		Camera& GetPrimaryCamera();
		bool HasPrimaryCamera();

		void CreateScene();

		//Scene& GetScene() { return *m_Scene; }

		std::string GetPath() { return m_Path; }
		inline std::string GetName() { return m_Name; }
		inline char* GetNameData() { return m_Name.data(); }
		void SetName(std::string name) { m_Name = name; }
		void SetPath(std::string path) { m_Path = path; }
	};
}