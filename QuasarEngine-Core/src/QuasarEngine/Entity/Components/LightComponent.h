#pragma once

#include <QuasarEngine/Entity/Component.h>
#include <QuasarEngine/Resources/Lights/PointLight.h>
#include <QuasarEngine/Resources/Lights/DirectionalLight.h>

namespace QuasarEngine
{
	class LightComponent : public Component
	{
	public:
		enum class LightType
		{
			NONE = 0,
			DIRECTIONAL = 1,
			POINT = 2
		};

		const char* item_type = "Directional Light";
		LightType lightType = LightType::DIRECTIONAL;

		//dirlight params
		glm::vec3 dir_color = { 1.0f, 1.0f, 1.0f };
		float dir_power = 1.0f;

		//pointlight params
		glm::vec3 point_color = { 1.0f, 1.0f, 1.0f };
		float point_attenuation = 1.0f;
		float point_power = 1.0f;

		LightComponent();
		LightComponent(LightType type);

		void SetType(LightType type);
	};
}