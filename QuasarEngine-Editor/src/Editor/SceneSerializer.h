#pragma once

#include <filesystem>
#include <QuasarEngine/Scene/Scene.h>

namespace QuasarEngine
{
	class SceneSerializer
	{
	public:
		SceneSerializer(Scene& scene, std::filesystem::path assetPath);

		void Serialize(const std::string& filepath);
		void SerializeRuntime(const std::string& filepath);
		bool Deserialize(const std::string& filepath);
		bool DeserializeRuntime(const std::string& filepath);
	private:
		Scene* m_Scene;
		std::filesystem::path m_AssetPath;
	};
}