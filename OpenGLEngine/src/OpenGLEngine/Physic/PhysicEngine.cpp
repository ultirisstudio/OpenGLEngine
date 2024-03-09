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

		reactphysics3d::PhysicsWorld::WorldSettings settings;
		settings.defaultVelocitySolverNbIterations = 20;
		settings.isSleepingEnabled = false;
		settings.gravity = reactphysics3d::Vector3(0, -9.81, 0);

		s_Data->world = s_Data->physicsCommon.createPhysicsWorld(settings);
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