#include <depch.h>
#include "LightComponent.h"

#include <OpenGLEngine/Renderer/Renderer.h>

namespace OpenGLEngine
{
	LightComponent::LightComponent()
	{
	}

	LightComponent::LightComponent(LightType type)
	{
		SetType(type);
	}

	void LightComponent::SetType(LightType type)
	{
		lightType = type;

		if (type == LightType::POINT)
		{
			item_type = "Point Light";
			lightType = LightType::POINT;
		}
		else if (type == LightType::DIRECTIONAL)
		{
			item_type = "Directional Light";
			lightType = LightType::DIRECTIONAL;
		}
	}
}