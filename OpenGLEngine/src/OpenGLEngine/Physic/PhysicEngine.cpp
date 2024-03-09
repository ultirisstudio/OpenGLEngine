#include "depch.h"
#include "PhysicEngine.h"

#include <reactphysics3d/reactphysics3d.h>

namespace OpenGLEngine
{
	struct PhysicEngineData
	{
		reactphysics3d::PhysicsCommon physicsCommon;
		reactphysics3d::PhysicsWorld* world;
	};

	static PhysicEngineData* s_Data = nullptr;

	void PhysicEngine::Init()
	{
		s_Data = new PhysicEngineData();

		s_Data->world = s_Data->physicsCommon.createPhysicsWorld();
	}

	void PhysicEngine::Shutdown()
	{
		s_Data->physicsCommon.destroyPhysicsWorld(s_Data->world);
	}

	void PhysicEngine::Update(double dt)
	{
		s_Data->world->update(dt);
	}

	reactphysics3d::PhysicsWorld* PhysicEngine::GetPhysicWorld()
	{
		return s_Data->world;
	}
}