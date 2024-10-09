#pragma once

namespace OpenGLEngine
{
	class Entity;

	class ScriptComponentPanel
	{
	public:
		ScriptComponentPanel() = default;
		~ScriptComponentPanel() = default;

		void Render(Entity entity);
	};
}
