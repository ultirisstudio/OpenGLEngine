#pragma once

class Entity;

namespace OpenGLEngine
{
	class CharacterControllerComponentPanel
	{
	public:
		CharacterControllerComponentPanel() = default;
		~CharacterControllerComponentPanel() = default;

		void Render(Entity* entity);
	};
}