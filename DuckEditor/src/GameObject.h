#pragma once

#include "DuckEngine.h"

namespace DuckEngine
{
	class GameObject
	{
	public:
		int m_Id;
		char m_Name[128];
		RenderModel m_RenderModel;
	public:
		GameObject(int id, Model& model);
		void Render();
		void Draw();
		void DrawInspector();
	private:
		float m_Position[3];
		float m_Rotation[3];
		float m_Scale[3];
	};
}