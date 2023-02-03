#pragma once

#include "DuckEngine.h"

namespace DuckEngine
{
	class GameObject
	{
	public:
		GameObject(int id, const std::string& name, Model& model, Texture& texture);

		int GetID() { return m_Id; }
		char* GetName() { return m_Name.data(); }

		void Render();
		void Draw();
		void DrawInspector();
	private:
		int m_Id;
		std::string m_Name;
		RenderModel m_RenderModel;
		Shader m_Shader;
		Texture m_Texture;

		float m_Position[3];
		float m_Rotation[3];
		float m_Scale[3];
	};
}