#include "Editor.h"
#include <glm/glm.hpp>

namespace DuckEngine
{
	Editor::Editor() : Layer("Editor")
	{
	}

	void Editor::Init()
	{

	}

	void Editor::OnUpdate()
	{
		Renderer::ClearColor(glm::vec4(0.5f, 0.2f, 0.6f, 1.0f));
		Renderer::Clear();
	}

	void Editor::OnImGuiRender()
	{
		
	}

	void Editor::OnEvent(Event& e)
	{
	}
}