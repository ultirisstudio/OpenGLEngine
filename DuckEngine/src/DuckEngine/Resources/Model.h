#pragma once

#include <DuckEngine/Resources/Mesh.h>

#include <assimp/scene.h>

#include <list>

#include <DuckEngine/Tools/Log.h>

namespace DuckEngine
{
	class Model
	{
	private:
		std::list<Mesh*> m_meshes;

		Mesh* loadMesh(const aiMesh* mesh, const aiScene* scene);
		void loadNode(const aiNode* node, const aiScene* scene);

	public:
		Model(const std::string& path);
		~Model();

		void draw() const;
	};
}