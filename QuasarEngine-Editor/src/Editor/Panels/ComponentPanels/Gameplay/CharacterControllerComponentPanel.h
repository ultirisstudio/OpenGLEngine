#pragma once


namespace QuasarEngine
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