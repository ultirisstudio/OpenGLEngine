#pragma once

#include <glm/glm.hpp>

#include <OpenGLEngine/ECS/Entity.h>
#include <OpenGLEngine/Scene/Scene.h>

namespace OpenGLEngine
{
	struct RaycastInfo
	{
		bool Hit;
		float Distance;
		glm::vec3 HitPoint;
		glm::vec3 Normal;
		uint32_t Entity;
	};

	class Raycast
	{
	public:
		static RaycastInfo RaycastAll(Scene* scene, const glm::vec3& origin, const glm::vec3& direction, float maxDistance = 1000.0f);
	};
}