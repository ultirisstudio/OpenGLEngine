#pragma once

#include <DuckEngine/Renderer/Renderer.h>
#include <DuckEngine/Resources/Material.h>
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
		glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), Rotation.x, { 1, 0, 0 }) * glm::rotate(glm::mat4(1.0f), Rotation.y, { 0, 1, 0 }) * glm::rotate(glm::mat4(1.0f), Rotation.z, { 0, 0, 1 });
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
	std::shared_ptr<DuckEngine::Material> m_Material;
public:
	MaterialComponent() = default;
	MaterialComponent(const std::string& path) { m_Material = DuckEngine::Renderer::CreateMaterial(path); }

	DuckEngine::Material& GetMaterial() { return *m_Material; }
	void SetMaterial(const std::string& path) { m_Material = DuckEngine::Renderer::CreateMaterial(path); }
};

class RenderComponent : public Component
{
private:
	DuckEngine::Shader m_Shader;
	std::shared_ptr<DuckEngine::Material> m_Material;
public:
	RenderComponent();

	void Draw();
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