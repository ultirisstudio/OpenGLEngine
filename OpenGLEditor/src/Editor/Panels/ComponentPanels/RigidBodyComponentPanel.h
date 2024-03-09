#pragma once

class Entity;

namespace OpenGLEngine
{
	class RigidBodyComponentPanel
	{
	public:
		RigidBodyComponentPanel() = default;
		~RigidBodyComponentPanel() = default;

		void Render(Entity* entity);
	};
}