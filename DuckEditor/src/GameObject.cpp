#include "GameObject.h"
#include "imgui.h"

namespace DuckEngine
{
	GameObject::GameObject(int id, Model& model) : m_Id(id), m_Name("Cube"), m_RenderModel(*Renderer::CreateRenderModel(&model, new Material()))
	{
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
		m_RenderModel.draw();
	}
	void GameObject::DrawInspector()
	{
		ImGui::Text("Information:");

		ImGui::Separator();

		std::stringstream sstm;
		sstm << "Object_" << m_Id;
		std::string result = sstm.str();
		ImGui::InputText(result.c_str(), m_Name, IM_ARRAYSIZE(m_Name));

		ImGui::Separator();

		ImGui::Text("Position");
		ImGui::SliderFloat3("Position", m_Position, -100.0f, 100.0f, "%0.1f");

		ImGui::Text("Rotation");
		ImGui::SliderFloat3("Rotation", m_Rotation, 0.0f, 180.0f, "%0.1f");
		ImGui::Text("Scale");
		ImGui::SliderFloat3("Scale", m_Scale, 0.1f, 100.0f, "%0.1f");
	}
}