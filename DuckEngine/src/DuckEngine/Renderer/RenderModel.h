#pragma once

#include <DuckEngine/Renderer/Element.h>
#include <DuckEngine/Resources/Model.h>

namespace DuckEngine
{
	class RenderModel : public Element
	{
	public:
		Model* m_Model;

	public:
		RenderModel(Material& material, Shader& shader, Model& model);

		void SetUniforms();
		void draw() const override;
	};
}