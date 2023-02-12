#pragma once

#include <ShaderGenerator/Materials/BPhongMaterial.hpp>
#include <ShaderGenerator/Materials/Material.hpp>

namespace sg
{
	class SG_API MaterialStructGenerator
	{
	public:
		static BPhongMaterial generateBPhongMaterial(const Material& material);
	};
}
