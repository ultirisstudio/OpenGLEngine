#pragma once

class Entity;

namespace OpenGLEngine
{
	class CapsuleColliderComponentPanel
	{
	public:
		CapsuleColliderComponentPanel() = default;
		~CapsuleColliderComponentPanel() = default;

		void Render(Entity* entity);
	};
}