#pragma once

#include <OpenGLEngine/Entity/Component.h>
#include <OpenGLEngine/Resources/Lights/PointLight.h>
#include <OpenGLEngine/Resources/Lights/DirectionalLight.h>

namespace OpenGLEngine
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
		glm::vec3 dir_diffuse = { 1.0f, 1.0f, 1.0f };
		glm::vec3 dir_specular = { 1.0f, 1.0f, 1.0f };

		//pointlight params
		glm::vec3 point_diffuse = { 1.0f, 1.0f, 1.0f };
		glm::vec3 point_specular = { 1.0f, 1.0f, 1.0f };
		float point_constant = 1.0f;
		float point_linear = 0.09f;
		float point_quadratic = 0.032f;

		LightComponent();
		LightComponent(LightType type);

		void SetType(LightType type);
	};
}