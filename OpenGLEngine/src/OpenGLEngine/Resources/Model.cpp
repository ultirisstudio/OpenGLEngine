#include "depch.h"
#include <OpenGLEngine/Resources/Model.h>

#include <OpenGLEngine/Tools/Log.h>

#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

namespace OpenGLEngine
{
	Mesh* Model::loadMesh(const aiMesh* mesh, const aiScene* scene)
	{
		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;

		vertices.reserve(mesh->mNumVertices);

		for (unsigned int i = 0; i < mesh->mNumVertices; i++)
		{
			Vertex vertex;

			vertex.position.x = mesh->mVertices[i].x;
			vertex.position.y = mesh->mVertices[i].y;
			vertex.position.z = mesh->mVertices[i].z;

			vertex.normal.x = mesh->mNormals[i].x;
			vertex.normal.y = mesh->mNormals[i].y;
			vertex.normal.z = mesh->mNormals[i].z;

			if (mesh->mTextureCoords[0])
			{
				vertex.texCoord.x = mesh->mTextureCoords[0][i].x;
				vertex.texCoord.y = mesh->mTextureCoords[0][i].y;
			}
			else
			{
				vertex.texCoord.x = 0.0f;
				vertex.texCoord.y = 0.0f;
			}

			vertices.push_back(vertex);
		}

		for (unsigned int i = 0; i < mesh->mNumFaces; i++)
		{
			aiFace face = mesh->mFaces[i];
			for (unsigned int j = 0; j < face.mNumIndices; j++)
				indices.push_back(face.mIndices[j]);
		}

		return new Mesh(vertices, indices);
	}

	void Model::loadNode(const aiNode* node, const aiScene* scene)
	{
		for (unsigned int i = 0; i < node->mNumMeshes; i++)
		{
			//m_meshes.push_back(loadMesh(scene->mMeshes[node->mMeshes[i]], scene));
			m_meshesMap[scene->mMeshes[node->mMeshes[i]]->mName.C_Str()] = loadMesh(scene->mMeshes[node->mMeshes[i]], scene);
		}

		for (unsigned int i = 0; i < node->mNumChildren; i++)
			loadNode(node->mChildren[i], scene);
	}

	Model::Model(const std::string& path) : m_Name("Unamed")
	{
		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_OptimizeMeshes | aiProcess_ImproveCacheLocality);

		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
		{
			Log::error("Failed to load model at", path, ":", importer.GetErrorString());
			return;
		}

		m_Name = scene->mRootNode->mName.C_Str();

		loadNode(scene->mRootNode, scene);
	}

	Model::Model(std::string name, std::vector<Vertex>& vertices, std::vector<unsigned int>& indices) : m_Name(name)
	{
		//m_meshes.push_back(new Mesh(vertices, indices));
		m_meshesMap[m_Name] = new Mesh(vertices, indices);
	}

	Model::~Model()
	{
		for (auto& [name, mesh] : m_meshesMap)
			delete mesh;

		//for (Mesh* mesh : m_meshes)
			//delete mesh;

		//m_meshes.clear();
		m_meshesMap.clear();
	}

	void Model::draw() const
	{
		//for (Mesh* mesh : m_meshes)
			//mesh->draw();

		for (auto& [name, mesh] : m_meshesMap)
			mesh->draw();
	}

	std::shared_ptr<Model> Model::CreateModel(const std::string& path)
	{
		return std::make_shared<Model>(path);
	}

	std::shared_ptr<Model> Model::CreateModel(std::string name, std::vector<Vertex>& vertices, std::vector<unsigned int>& indices)
	{
		return std::make_shared<Model>(name, vertices, indices);
	}

	std::unordered_map<std::string, Mesh*>& Model::GetMeshes()
	{
		return m_meshesMap;
	}
}