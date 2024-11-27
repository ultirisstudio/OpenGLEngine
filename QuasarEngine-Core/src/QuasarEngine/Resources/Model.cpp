#include "qepch.h"

#include <filesystem>

#include <QuasarEngine/Resources/Model.h>
#include <QuasarEngine/Resources/Materials/Material.h>

#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

#include <assimp/pbrmaterial.h>

namespace QuasarEngine
{
	Mesh* Model::loadMesh(const aiMesh* mesh, const aiScene* scene)
	{
		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;

		vertices.reserve(mesh->mNumVertices);

		//std::cout << "Name: " << mesh->mName.C_Str() << std::endl;
		//std::cout << "NumColorChannels: " << mesh->GetNumColorChannels() << std::endl;
		//std::cout << "NumUVChannels: " << mesh->GetNumUVChannels() << std::endl;
		//std::cout << "MaterialIndex: " << mesh->mMaterialIndex << std::endl;

		//aiString fileGLTFBaseColorTexture;
		//scene->mMaterials[mesh->mMaterialIndex]->GetTextureCount(AI_MATKEY_GLTF_PBRMETALLICROUGHNESS_BASE_COLOR_TEXTURE, &fileGLTFBaseColorTexture);

		//std::cout << "BaseColor: " << fileGLTFBaseColorTexture.C_Str() << std::endl;
		//std::cout << "BaseColor: " << scene->mMaterials[mesh->mMaterialIndex]->GetTextureCount(aiTextureType_DIFFUSE) << std::endl;
		//aiTextureType type = aiTextureType_DIFFUSE;

		for (unsigned int i = 0; i < mesh->mNumVertices; i++)
		{
			Vertex vertex;

			vertex.position.x = mesh->mVertices[i].x;
			vertex.position.y = mesh->mVertices[i].y;
			vertex.position.z = mesh->mVertices[i].z;

			if (mesh->HasNormals())
			{
				vertex.normal.x = mesh->mNormals[i].x;
				vertex.normal.y = mesh->mNormals[i].y;
				vertex.normal.z = mesh->mNormals[i].z;
			}
			else
			{
				vertex.normal = glm::vec3(0.0f);
			}

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

		/////////////////////////////// MATERIAL ///////////////////////////////

		MaterialSpecification mat;

		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

		unsigned int nbDiffuseTexture = material->GetTextureCount(aiTextureType_DIFFUSE);
		//std::cout << "Diffuse Texture: " << nbDiffuseTexture << std::endl;
		for (unsigned int i = 0; i < nbDiffuseTexture; i++)
		{
			aiString str;
			material->GetTexture(aiTextureType_DIFFUSE, i, &str);
			//std::cout << str.C_Str() << std::endl;
			std::filesystem::path diffusePath = str.C_Str();
			mat.AlbedoTexture = diffusePath.filename().string();
		}

		unsigned int nbNormalTexture = material->GetTextureCount(aiTextureType_NORMALS);
		//std::cout << "Normal Texture: " << nbNormalTexture << std::endl;
		for (unsigned int i = 0; i < nbNormalTexture; i++)
		{
			aiString str;
			material->GetTexture(aiTextureType_NORMALS, i, &str);
			//std::cout << str.C_Str() << std::endl;
			std::filesystem::path normalPath = str.C_Str();
			mat.NormalTexture = normalPath.filename().string();
		}

		//unsigned int nbAOTexture = material->GetTextureCount(aiTextureType_LIGHTMAP);
		//std::cout << "AO Texture: " << nbAOTexture << std::endl;

		//return new Mesh(vertices, indices, mat);
		return new Mesh(vertices, indices);
	}

	void Model::loadNode(const aiNode* node, const aiScene* scene)
	{
		for (unsigned int i = 0; i < node->mNumMeshes; i++)
		{
			m_meshesMap[scene->mMeshes[node->mMeshes[i]]->mName.C_Str()] = loadMesh(scene->mMeshes[node->mMeshes[i]], scene);
		}

		for (unsigned int i = 0; i < node->mNumChildren; i++)
			loadNode(node->mChildren[i], scene);
	}

	Model::Model(const std::string& path) : m_Name("Unamed")
	{
		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_SortByPType); // aiProcess_PreTransformVertices | aiProcess_GenSmoothNormals | aiProcess_CalcTangentSpace | aiProcess_FixInfacingNormals | aiProcess_GenUVCoords | aiProcess_FlipUVs | aiProcess_ImproveCacheLocality // | aiProcess_OptimizeMeshes

		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
		{
			std::cout << "Failed to load model at " << path << " : " << importer.GetErrorString() << std::endl;
			return;
		}

		m_Name = scene->mRootNode->mName.C_Str();

		loadNode(scene->mRootNode, scene);
	}

	Model::Model(std::string name, std::vector<Vertex>& vertices, std::vector<unsigned int>& indices) : m_Name(name)
	{
		m_meshesMap[m_Name] = new Mesh(vertices, indices);
	}

	Model::~Model()
	{
		for (auto& [name, mesh] : m_meshesMap)
			delete mesh;

		m_meshesMap.clear();
	}

	void Model::draw() const
	{
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

	Mesh* Model::GetMesh(const std::string& name)
	{
		return m_meshesMap[name];
	}

	std::unordered_map<std::string, Mesh*>& Model::GetMeshes()
	{
		return m_meshesMap;
	}
}