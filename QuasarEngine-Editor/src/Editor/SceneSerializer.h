#pragma once

#include <filesystem>

#include "SceneObject.h"

namespace QuasarEngine
{
	class SceneSerializer
	{
	public:
		SceneSerializer(SceneObject& sceneObject, std::filesystem::path assetPath);

		void Serialize(const std::string& filepath);
		void SerializeRuntime(const std::string& filepath);
		bool Deserialize(const std::string& filepath);
		bool DeserializeRuntime(const std::string& filepath);
	private:
		SceneObject* m_SceneObject;
		std::filesystem::path m_AssetPath;
	};
}