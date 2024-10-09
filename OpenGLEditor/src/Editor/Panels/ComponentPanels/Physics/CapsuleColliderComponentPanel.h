#pragma once

namespace OpenGLEngine
{
	class Entity;

	class CapsuleColliderComponentPanel
	{
	public:
		CapsuleColliderComponentPanel() = default;
		~CapsuleColliderComponentPanel() = default;

		void Render(Entity entity);
	};
}