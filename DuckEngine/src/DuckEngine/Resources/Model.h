#pragma once

#include <DuckEngine/Resources/Resource.h>
#include <DuckEngine/Resources/Mesh.h>

#include <assimp/scene.h>

#include <list>

#include <DuckEngine/Tools/Log.h>

namespace DuckEngine
{
	class Model : public Resource
	{
	private:
		std::list<Mesh*> m_meshes;

		Mesh* loadMesh(const aiMesh* mesh, const aiScene* scene);
		void loadNode(const aiNode* node, const aiScene* scene);

	public:
		void load(const std::string& path) override;
		void free() override;

		void draw() const;
	};
}