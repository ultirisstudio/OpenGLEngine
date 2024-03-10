#pragma once

class Entity;

namespace OpenGLEngine
{
	class ColliderComponentPanel
	{
	public:
		ColliderComponentPanel() = default;
		~ColliderComponentPanel() = default;

		void Render(Entity* entity);
	};
}