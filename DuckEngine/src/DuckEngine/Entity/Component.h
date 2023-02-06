#pragma once

#include <DuckEngine/Renderer/Renderer.h>
#include <DuckEngine/Resources/Material.h>
#include <DuckEngine/Resources/Model.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>

#define MAX_COMPONENTS 10

#include "Entity.h"

namespace DuckEngine
{
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
		virtual ~Component();
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

			glm::mat4 rotation = glm::toMat4(glm::quat(Rotation));

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
		std::shared_ptr<Model> m_Model;
	public:
		ModelComponent() = default;
		ModelComponent(const std::string& path) { m_Model = Renderer::CreateModel(path); }
		
		Model& GetModel() { return *m_Model; }
		void SetModel(const std::string& path) { m_Model = Renderer::CreateModel(path); }
	};

	class MaterialComponent : public Component
	{
	private:
		std::shared_ptr<Material> m_Material;
	public:
		MaterialComponent() = default;
		MaterialComponent(const std::string& path) { m_Material = Renderer::CreateMaterial(path); }

		Material& GetMaterial() { return *m_Material; }
		void SetMaterial(const std::string& path) { m_Material = Renderer::CreateMaterial(path); }
	};

	class RenderComponent : public Component
	{
	private:
		Shader m_Shader;
	public:
		RenderComponent() : m_Shader()
		{
			m_Shader.LoadFromFile("Shaders/texture.vert", "Shaders/texture.frag");
		}

		Shader& GetShader() { return m_Shader; }
	};

	class SkyboxComponent : public Component
	{
	private:
		glm::vec3 Position = { 0.0f,0.0f,0.0f };
		glm::vec3 Rotation = { 0.0f,0.0f,0.0f };
		glm::vec3 Scale = { 15.0f,15.0f,15.0f };

	public:
		std::shared_ptr<Model> m_Model;
		CubeMap m_CubeMap;
		Shader m_CubeMapShader;

		SkyboxComponent(const std::string& path)
		{
			m_CubeMapShader.LoadFromFile("Shaders/cubemap.vert", "Shaders/cubemap.frag");
			m_Model = Renderer::CreateModel(path);
			m_CubeMap.Load({ "Assets/Skybox/right.jpg", "Assets/Skybox/left.jpg", "Assets/Skybox/top.jpg", "Assets/Skybox/bottom.jpg", "Assets/Skybox/front.jpg", "Assets/Skybox/back.jpg" });
		}

		glm::mat4 GetTransform() const {

			glm::mat4 rotation = glm::toMat4(glm::quat(Rotation));

			return glm::translate(glm::mat4(1.f), Position) * rotation * glm::scale(glm::mat4(1.0f), Scale);
		}
	};
}