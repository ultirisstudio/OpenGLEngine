#include "depch.h"
#include "GameObject.h"
#include "imgui.h"
#include <glad/glad.h>

namespace DuckEngine
{
	GameObject::GameObject(int id, const std::string& name, Model& model, Texture& texture) : m_Id(id), m_Name(name), m_RenderModel(*Renderer::CreateRenderModel(&model, &m_Shader, new Material())), m_Texture(texture)
	{
		m_Shader.LoadFromFile("Shaders/texture.vert", "Shaders/texture.frag");

		m_Position[0] = 0.0f;
		m_Position[1] = 0.0f;
		m_Position[2] = 0.0f;

		m_Rotation[0] = 0.0f;
		m_Rotation[1] = 0.0f;
		m_Rotation[2] = 0.0f;

		m_Scale[0] = 1.0f;
		m_Scale[1] = 1.0f;
		m_Scale[2] = 1.0f;
	}

	void GameObject::Render()
	{
		m_RenderModel.setPosition(glm::vec3(m_Position[0], m_Position[1], m_Position[2]));
		m_RenderModel.setRotation(glm::vec3(m_Rotation[0], m_Rotation[1], m_Rotation[2]));
		m_RenderModel.setScale(glm::vec3(m_Scale[0], m_Scale[1], m_Scale[2]));
	}

	void GameObject::Draw()
	{
		glActiveTexture(GL_TEXTURE0);
		m_Texture.bind();
		m_RenderModel.SetUniforms();
		m_Shader.setUniform("uTexture", 0);
		m_RenderModel.draw();
	}
	void GameObject::DrawInspector()
	{
		ImGui::Text("Information:");

		ImGui::Separator();

		std::stringstream sstm;
		sstm << "Object_" << m_Id;
		std::string result = sstm.str();
		ImGui::InputText(result.c_str(), GetName(), 10);

		ImGui::Separator();

		ImGui::Text("Position");
		ImGui::DragFloat3("Position", m_Position, 0.1f, -1000.0f, 1000.0f);
		ImGui::Text("Rotation");
		ImGui::DragFloat3("Rotation", m_Rotation, 0.1f, 0.0f, 180.0f);
		ImGui::Text("Scale");
		ImGui::DragFloat3("Scale", m_Scale, 0.1f, 100.0f);
	}
}