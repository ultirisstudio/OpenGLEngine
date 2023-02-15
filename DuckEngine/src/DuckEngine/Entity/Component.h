#pragma once

#include <DuckEngine/Renderer/Renderer.h>
#include <DuckEngine/Resources/Model.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>

#define MAX_COMPONENTS 10

#include "Entity.h"

class Entity;

inline size_t GetComponentTypeID() {
	static size_t lastID = 0;
	return lastID++;
}

template<typename T>
inline size_t GetComponentTypeID() noexcept {
	static size_t typeID = GetComponentTypeID();
	return typeID;
}

class Component
{
public:
	Entity* entity;

	Component() : entity(nullptr) {}
	virtual ~Component() {}
};

class TransformComponent : public Component
{
public:
	glm::vec3 Position = { 0.0f,0.0f,0.0f };
	glm::vec3 Rotation = { 0.0f,0.0f,0.0f };
	glm::vec3 Scale = { 1.0f,1.0f,1.0f };

	TransformComponent() = default;
	TransformComponent(const glm::vec3& position) : Position(position) {}

	glm::mat4 GetTransform() const {

		//glm::mat4 rotation = glm::toMat4(glm::quat(Rotation));
		glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), glm::radians(Rotation.x), { 1, 0, 0 }) * glm::rotate(glm::mat4(1.0f), glm::radians(Rotation.y), { 0, 1, 0 }) * glm::rotate(glm::mat4(1.0f), glm::radians(Rotation.z), { 0, 0, 1 });
		return glm::translate(glm::mat4(1.f), Position) * rotation * glm::scale(glm::mat4(1.0f), Scale);
	}

	glm::quat GetQuatRotation() {
		glm::quat rotation = glm::quat(Rotation);
		return rotation;
	}
};

class ModelComponent : public Component
{
private:
	std::shared_ptr<DuckEngine::Model> m_Model;
public:
	ModelComponent() = default;
	ModelComponent(const std::string& path) { m_Model = DuckEngine::Renderer::CreateModel(path); }
		
	DuckEngine::Model& GetModel() { return *m_Model; }
	void SetModel(const std::string& path) { m_Model = DuckEngine::Renderer::CreateModel(path); }

	DuckEngine::Model* GetPtr() { return m_Model.get(); }
};

class MaterialComponent : public Component
{
private:
	std::shared_ptr<DuckEngine::Texture> m_DefaultTexture;
	std::shared_ptr<DuckEngine::Texture> m_NoTexture;

	std::shared_ptr<DuckEngine::Material> m_Material;
public:
	//glm::vec3 DiffuseColor = { 0.0f,0.0f,0.0f };
	//glm::vec3 AmbientColor = { 0.0f,0.0f,0.0f };
	bool UseDiffuseTexture;
	bool UseSpecularTexture;

	MaterialComponent()
	{
		m_Material = DuckEngine::Renderer::CreateMaterial();
		m_DefaultTexture = DuckEngine::Renderer::CreateTexture("Assets/Textures/white_texture.jpg");
		m_NoTexture = DuckEngine::Renderer::CreateTexture("Assets/Textures/3d-modeling.png");
	}

	DuckEngine::Material& GetMaterial() { return *m_Material; }

	DuckEngine::Texture& GetDefaultTexture() { return *m_DefaultTexture; }
	DuckEngine::Texture& GetNoTexture() { return *m_NoTexture; }

	DuckEngine::Texture& GetEditorDiffuseTexture()
	{
		if (m_Material->hasTexture("diffuse"))
		{
			return *m_Material->getTexture("diffuse");
		}

		return *m_NoTexture;
	}

	DuckEngine::Texture& GetEditorSpecularTexture()
	{
		if (m_Material->hasTexture("specular"))
		{
			return *m_Material->getTexture("specular");
		}

		return *m_NoTexture;
	}
};

class RenderComponent : public Component
{
private:
	DuckEngine::Shader m_Shader;
	std::shared_ptr<DuckEngine::Material> m_Material;
public:
	RenderComponent();

	void Draw();
	void GenerateShader();
	DuckEngine::Shader& GetShader() { return m_Shader; }
};

class SkyboxComponent : public Component
{
private:
	glm::vec3 Position = { 0.0f,0.0f,0.0f };
	glm::vec3 Rotation = { 0.0f,0.0f,0.0f };
	glm::vec3 Scale = { 15.0f,15.0f,15.0f };

public:
	std::shared_ptr<DuckEngine::Model> m_Model;
	DuckEngine::CubeMap m_CubeMap;
	DuckEngine::Shader m_CubeMapShader;

	SkyboxComponent(const std::string& path)
	{
		m_CubeMapShader.LoadFromFile("Shaders/cubemap.vert", "Shaders/cubemap.frag");
		m_Model = DuckEngine::Renderer::CreateModel(path);
		m_CubeMap.Load({ "Assets/Skybox/right.jpg", "Assets/Skybox/left.jpg", "Assets/Skybox/top.jpg", "Assets/Skybox/bottom.jpg", "Assets/Skybox/front.jpg", "Assets/Skybox/back.jpg" });
	}

	glm::mat4 GetTransform() const {

		glm::mat4 rotation = glm::toMat4(glm::quat(Rotation));

		return glm::translate(glm::mat4(1.f), Position) * rotation * glm::scale(glm::mat4(1.0f), Scale);
	}
};