#pragma once

namespace reactphysics3d {
	extern "C" {
		class PhysicsWorld;
		class PhysicsCommon;
		class DebugRenderer;
	}
}

namespace QuasarEngine
{
	class PhysicEngine
	{
	public:
		static void Init();
		static void Shutdown();

		static void Update(double dt);

		static void Reload();
		
		static reactphysics3d::PhysicsWorld* GetPhysicWorld();
		static reactphysics3d::PhysicsCommon* GetPhysicsCommon();

		static reactphysics3d::DebugRenderer& GetDebugRenderer();
	};
}
