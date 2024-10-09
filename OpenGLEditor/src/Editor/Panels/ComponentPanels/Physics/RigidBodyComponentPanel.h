#pragma once

namespace OpenGLEngine
{
	class Entity;

	class RigidBodyComponentPanel
	{
	public:
		RigidBodyComponentPanel() = default;
		~RigidBodyComponentPanel() = default;

		void Render(Entity entity);
	};
}