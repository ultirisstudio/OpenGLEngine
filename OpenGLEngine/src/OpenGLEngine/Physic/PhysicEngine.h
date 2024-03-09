#pragma once

namespace reactphysics3d {
	extern "C" {
		class PhysicsWorld;
	}
}

namespace OpenGLEngine
{
	class PhysicEngine
	{
	public:
		static void Init();
		static void Shutdown();

		static void Update(double dt);
		
		static reactphysics3d::PhysicsWorld* GetPhysicWorld();
	};
}
