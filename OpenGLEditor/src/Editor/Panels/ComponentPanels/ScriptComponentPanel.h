#pragma once

class Entity;

namespace OpenGLEngine
{
	class ScriptComponentPanel
	{
	public:
		ScriptComponentPanel() = default;
		~ScriptComponentPanel() = default;

		void Render(Entity* entity);
	};
}
