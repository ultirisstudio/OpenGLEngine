#pragma once

#include <OpenGLEngine/ECS/Component.h>

namespace OpenGLEngine
{
	class MeshRendererComponent : public Component
	{
	public:
		MeshRendererComponent() = default;
		~MeshRendererComponent() = default;

		bool m_Rendered = true;
	};
}