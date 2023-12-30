#pragma once

#include <OpenGLEngine/Entity/Component.h>

namespace OpenGLEngine
{
	class LightComponent : public Component
	{
	public:
		enum class LightType
		{
			DIRECTIONAL = 0,
			POINT = 1
		};

		LightType lightType = LightType::DIRECTIONAL;

		//static int nbPointLight;
		int id = 0;

		//dirlight params
		glm::vec3 dir_direction = { 1.0f, 1.0f, 1.0f };
		glm::vec3 dir_ambient = { 1.0f, 1.0f, 1.0f };
		glm::vec3 dir_diffuse = { 1.0f, 1.0f, 1.0f };
		glm::vec3 dir_specular = { 1.0f, 1.0f, 1.0f };

		//pointlight params
		glm::vec3 point_position = { 1.0f, 1.0f, 1.0f };
		glm::vec3 point_ambient = { 1.0f, 1.0f, 1.0f };
		glm::vec3 point_diffuse = { 1.0f, 1.0f, 1.0f };
		glm::vec3 point_specular = { 1.0f, 1.0f, 1.0f };
		float point_constant = 0;
		float point_linear = 0;
		float point_quadratic = 0;

		LightComponent()
		{
			//nbPointLight = 0;
		}

		void SetType(LightType type)
		{
			lightType = type;

			if (type == LightType::POINT)
			{
				//id = nbPointLight;
				//nbPointLight++;
			}
		}
	};
}