#pragma once

/*namespace rp3d
{
	class PhysicsCommon;
	class PhysicsWorld;
}*/

namespace OpenGLEngine
{
	class PhysicsEngine
	{
	private:
		//rp3d::PhysicsCommon* m_physicsCommon;
		//rp3d::PhysicsWorld* m_physicsWorld;
	public:
		PhysicsEngine();
		~PhysicsEngine();

		void Init();
		void Update(float deltaTime);
		void Shutdown();
	};
}