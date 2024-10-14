#include "qepch.h"
#include "PhysicEngine.h"

#include <reactphysics3d/reactphysics3d.h>

namespace QuasarEngine
{
	struct PhysicEngineData
	{
		reactphysics3d::PhysicsCommon physicsCommon;
		reactphysics3d::PhysicsWorld* world;

		reactphysics3d::DebugRenderer* debugRenderer;
	};

	static PhysicEngineData* s_Data = nullptr;

	void PhysicEngine::Init()
	{
		s_Data = new PhysicEngineData();

		reactphysics3d::PhysicsWorld::WorldSettings settings;
		settings.gravity = reactphysics3d::Vector3(0, -9.81, 0);

		s_Data->world = s_Data->physicsCommon.createPhysicsWorld(settings);

		s_Data->world->setIsDebugRenderingEnabled(true);
		s_Data->debugRenderer = &s_Data->world->getDebugRenderer();
		s_Data->debugRenderer->setIsDebugItemDisplayed(reactphysics3d::DebugRenderer::DebugItem::CONTACT_POINT, true);
		s_Data->debugRenderer->setIsDebugItemDisplayed(reactphysics3d::DebugRenderer::DebugItem::CONTACT_NORMAL, true);
		s_Data->debugRenderer->setIsDebugItemDisplayed(reactphysics3d::DebugRenderer::DebugItem::COLLISION_SHAPE, true);
		s_Data->debugRenderer->setIsDebugItemDisplayed(reactphysics3d::DebugRenderer::DebugItem::COLLIDER_AABB, true);
		s_Data->debugRenderer->setIsDebugItemDisplayed(reactphysics3d::DebugRenderer::DebugItem::COLLIDER_BROADPHASE_AABB, true);
	}

	void PhysicEngine::Shutdown()
	{
		s_Data->physicsCommon.destroyPhysicsWorld(s_Data->world);
	}

	void PhysicEngine::Update(double dt)
	{
		s_Data->world->update(dt);
	}

	void PhysicEngine::Reload()
	{
		Init();
	}

	reactphysics3d::PhysicsWorld* PhysicEngine::GetPhysicWorld()
	{
		return s_Data->world;
	}

	reactphysics3d::PhysicsCommon* PhysicEngine::GetPhysicsCommon()
	{
		return &s_Data->physicsCommon;
	}

	reactphysics3d::DebugRenderer& PhysicEngine::GetDebugRenderer()
	{
		return *s_Data->debugRenderer;
	}
}