#pragma once

#include <DuckEngine/Resources/Material.h>
#include <DuckEngine/Shader/Shader.h>
#include <glm/glm.hpp>

namespace DuckEngine
{
	class Drawable
	{
	protected:
		Material* m_Material;
		Shader* m_Shader;

	public:
		Drawable(Material& material, Shader& shader);
		virtual ~Drawable() {};

		Material& getMaterial() const;
		Shader& getShader() const;

		virtual void draw() const = 0;
	};
}