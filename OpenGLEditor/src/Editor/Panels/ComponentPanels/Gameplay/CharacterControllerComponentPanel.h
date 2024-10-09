#pragma once


namespace OpenGLEngine
{
	class Entity;

	class CharacterControllerComponentPanel
	{
	public:
		CharacterControllerComponentPanel() = default;
		~CharacterControllerComponentPanel() = default;

		void Render(Entity entity);
	};
}