#pragma once

#include <ShaderGenerator/Materials/MaterialProperty.hpp>

namespace sg
{
	struct SG_API BPhongMaterial
	{
		MaterialProperty ambient;
		MaterialProperty diffuse;
		MaterialProperty specular;
		float shininess;

		bool hasAnyTexture;
	};
}
