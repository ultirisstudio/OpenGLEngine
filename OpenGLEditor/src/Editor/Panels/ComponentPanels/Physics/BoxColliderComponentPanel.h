#pragma once

class Entity;

namespace OpenGLEngine
{
	class BoxColliderComponentPanel
	{
	public:
		BoxColliderComponentPanel() = default;
		~BoxColliderComponentPanel() = default;

		void Render(Entity* entity);
	};
}