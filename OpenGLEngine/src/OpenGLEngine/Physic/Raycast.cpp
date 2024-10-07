#include "depch.h"
#include "Raycast.h"

#include <reactphysics3d/reactphysics3d.h>

#include <OpenGLEngine/Renderer/Renderer.h>
#include <OpenGLEngine/Entity/Entity.h>
#include <OpenGLEngine/Entity/Components/IDComponent.h>
#include <OpenGLEngine/Entity/Components/Physics/RigidBodyComponent.h>
#include <OpenGLEngine/Entity/Components/Gameplay/CharacterControllerComponent.h>

namespace OpenGLEngine
{
	RaycastInfo Raycast::RaycastAll(Scene* scene, const glm::vec3& origin, const glm::vec3& direction, float maxDistance)
	{
		RaycastInfo result;
		result.Distance = maxDistance;

		Entity hitEntity = entt::null;
		float distance = maxDistance;

		reactphysics3d::RaycastInfo raycastInfo;

		bool hit = false;

		for (auto e : scene->GetAllEntitiesWith<IDComponent>())
		{
			reactphysics3d::RigidBody* rigidBody = nullptr;
			Entity entity{ e, Renderer::m_SceneData.m_Scene };
			if (entity.HasComponent<RigidBodyComponent>())
			{
				rigidBody = entity.GetComponent<RigidBodyComponent>().GetRigidBody();
			}
			if (entity.HasComponent<CharacterControllerComponent>())
			{
				rigidBody = entity.GetComponent<CharacterControllerComponent>().GetRigidBody();
			}

			if (rigidBody == nullptr)
				continue;

			glm::vec3 end = origin + direction * maxDistance;

			reactphysics3d::Vector3 startPoint(origin.x, origin.y, origin.z);
			reactphysics3d::Vector3 endPoint(end.x, end.y, end.z);
			reactphysics3d::Ray ray(startPoint, endPoint);

			bool isHit = rigidBody->raycast(ray, raycastInfo);

			if (isHit)
			{
				if (raycastInfo.hitFraction < distance)
				{
					distance = raycastInfo.hitFraction;
					hitEntity = entity;
					hit = true;
				}
			}
		}

		if (hit)
		{
			result.Hit = true;
			result.Distance = distance;
			result.Entity = hitEntity.GetUUID();
			result.HitPoint = glm::vec3(raycastInfo.worldPoint.x, raycastInfo.worldPoint.y, raycastInfo.worldPoint.z);
			result.Normal = glm::vec3(raycastInfo.worldNormal.x, raycastInfo.worldNormal.y, raycastInfo.worldNormal.z);
		}
		else
		{
			result.Hit = false;
			result.Distance = 0;
			result.Entity = 0;
			result.HitPoint = glm::vec3(0.0f);
			result.Normal = glm::vec3(0.0f);
		}

		return result;
	}
}