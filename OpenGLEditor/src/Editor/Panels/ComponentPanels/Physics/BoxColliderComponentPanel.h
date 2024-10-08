#pragma once

namespace OpenGLEngine
{
	class Entity;

	class BoxColliderComponentPanel
	{
	public:
		BoxColliderComponentPanel() = default;
		~BoxColliderComponentPanel() = default;

		void Render(Entity entity);
	};
}