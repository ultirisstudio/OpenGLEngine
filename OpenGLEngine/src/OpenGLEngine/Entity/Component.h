#pragma once

#include <OpenGLEngine/Renderer/Renderer.h>
#include <OpenGLEngine/Resources/Model.h>
#include <OpenGLEngine/Resources/Materials/Material.h>
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

	void SetPosition(const glm::vec3& position) { Position = position; }
	void SetRotation(const glm::vec3& rotation) { Rotation = rotation; }
	void SetScale(const glm::vec3& scale) { Scale = scale; }

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
	std::shared_ptr<OpenGLEngine::Model> m_Model;
public:
	ModelComponent() = default;
	ModelComponent(const std::string& path) { m_Model = OpenGLEngine::Renderer::m_SceneData.m_ResourceManager.getModel(path); }
		
	OpenGLEngine::Model& GetModel() { return *m_Model; }
	void SetModel(const std::string& path) { m_Model = OpenGLEngine::Renderer::m_SceneData.m_ResourceManager.getModel(path); }

	OpenGLEngine::Model* GetPtr() { return m_Model.get(); }
};

class MaterialComponent : public Component
{
private:
	std::shared_ptr<OpenGLEngine::Texture> m_DefaultTexture;
	std::shared_ptr<OpenGLEngine::Texture> m_NoTexture;

	std::shared_ptr<OpenGLEngine::Material> m_Material;
public:
	bool UseDiffuseTexture;
	bool UseSpecularTexture;

	const char* current_item = "BPhong";

	MaterialComponent();

	void InitializeMaterial();

	OpenGLEngine::Material& GetMaterial() { return *m_Material; }

	OpenGLEngine::Texture& GetDefaultTexture() { return *m_DefaultTexture; }
	OpenGLEngine::Texture& GetNoTexture() { return *m_NoTexture; }

	OpenGLEngine::Texture& GetEditorDiffuseTexture()
	{
		if (m_Material->hasTexture("diffuse"))
		{
			return *m_Material->getTexture("diffuse");
		}

		return *m_NoTexture;
	}

	OpenGLEngine::Texture& GetEditorSpecularTexture()
	{
		if (m_Material->hasTexture("specular"))
		{
			return *m_Material->getTexture("specular");
		}

		return *m_NoTexture;
	}
};

class SkyboxComponent : public Component
{
private:
	std::shared_ptr<OpenGLEngine::Model> m_Model;
	OpenGLEngine::CubeMap m_CubeMap;
	OpenGLEngine::Shader m_CubeMapShader;
public:
	SkyboxComponent(const std::string& path)
	{
		m_CubeMapShader.LoadFromFile("Shaders/cubemap.vert", "Shaders/cubemap.frag");
		m_Model = OpenGLEngine::Model::CreateModel(path);
		m_CubeMap.Load({ "Assets/Skybox/right.jpg", "Assets/Skybox/left.jpg", "Assets/Skybox/top.jpg", "Assets/Skybox/bottom.jpg", "Assets/Skybox/front.jpg", "Assets/Skybox/back.jpg" });
	}

	OpenGLEngine::CubeMap* GetCubeMap()
	{
		return &m_CubeMap;
	}

	OpenGLEngine::Shader* GetCubeMapShader()
	{
		return &m_CubeMapShader;
	}

	OpenGLEngine::Model* GetModel()
	{
		return m_Model.get();
	}
};

class RenderComponent : public Component
{
private:
	OpenGLEngine::Shader m_Shader;
public:
	bool m_CanDraw;

	RenderComponent();

	void Draw();
	void GenerateShader();

	OpenGLEngine::Shader& GetShader() { return m_Shader; }
};