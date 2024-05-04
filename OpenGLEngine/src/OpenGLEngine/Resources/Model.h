#pragma once

#include <list>
#include <unordered_map>

#include <OpenGLEngine/Resources/Mesh.h>
#include <OpenGLEngine/Tools/Log.h>
#include <OpenGLEngine/Asset/Asset.h>

struct aiScene;
struct aiNode;
struct aiMesh;

namespace OpenGLEngine
{
	class Model : public Asset
	{
	private:
		std::unordered_map<std::string, Mesh*> m_meshesMap;

		std::string m_Name;

		Mesh* loadMesh(const aiMesh* mesh, const aiScene* scene);
		void loadNode(const aiNode* node, const aiScene* scene);
	public:
		Model(const std::string& path);
		Model(std::string name, std::vector<Vertex>& vertices, std::vector<unsigned int>& indices);
		~Model();

		static std::shared_ptr<Model> CreateModel(const std::string& path);
		static std::shared_ptr<Model> CreateModel(std::string name, std::vector<Vertex>& vertices, std::vector<unsigned int>& indices);

		Mesh* GetMesh(const std::string& name);
		std::unordered_map<std::string, Mesh*>& GetMeshes();

		void draw() const;

		static AssetType GetStaticType() { return AssetType::MODEL; }
		virtual AssetType GetType() override { return GetStaticType(); }
	};
}