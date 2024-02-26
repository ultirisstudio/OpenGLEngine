#pragma once

#include <OpenGLEngine/Entity/Component.h>

namespace OpenGLEngine
{
	class ScriptComponent : public Component
	{
	public:
		std::string m_Name;

		ScriptComponent() = default;
		ScriptComponent(const ScriptComponent&) = default;
	};
}