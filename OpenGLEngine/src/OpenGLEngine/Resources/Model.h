#pragma once

#include <OpenGLEngine/Resources/Mesh.h>

#include <list>

#include <OpenGLEngine/Tools/Log.h>

struct aiScene;
struct aiNode;
struct aiMesh;

namespace OpenGLEngine
{
	class Model
	{
	private:
		std::list<Mesh*> m_meshes;

		Mesh* loadMesh(const aiMesh* mesh, const aiScene* scene);
		void loadNode(const aiNode* node, const aiScene* scene);

	public:
		Model(const std::string& path);
		Model(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices);
		~Model();

		static std::shared_ptr<Model> CreateModel(const std::string& path);
		static std::shared_ptr<Model> CreateModel(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices);

		void draw() const;
	};
}