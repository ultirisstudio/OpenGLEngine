#pragma once

#include <OpenGLEngine/Scene/Scene.h>

namespace OpenGLEngine
{
	class SceneSerializer
	{
	public:
		SceneSerializer(Scene& scene) : m_Scene(&scene) {}

		void Serialize(const std::string& filepath);
		void SerializeRuntime(const std::string& filepath);
		bool Deserialize(const std::string& filepath);
		bool DeserializeRuntime(const std::string& filepath);
	private:
		Scene* m_Scene;
	};
}