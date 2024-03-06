#pragma once

class Entity;

namespace OpenGLEngine
{
	class LightComponentPanel
	{
	public:
		LightComponentPanel() = default;
		~LightComponentPanel() = default;

		void Render(Entity* entity);
	};
}
