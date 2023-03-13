#pragma once

#include <OpenGLEngine/Resources/Mesh.h>

#include <assimp/scene.h>

#include <list>

#include <OpenGLEngine/Tools/Log.h>

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
		~Model();

		static std::shared_ptr<Model> CreateModel(const std::string& path);

		void draw() const;
	};
}