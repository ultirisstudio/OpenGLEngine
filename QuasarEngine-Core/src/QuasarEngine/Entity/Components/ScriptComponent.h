#pragma once

#include <QuasarEngine/Entity/Component.h>

namespace QuasarEngine
{
	class ScriptComponent : public Component
	{
	public:
		std::string m_Name;

		ScriptComponent() = default;
		ScriptComponent(const ScriptComponent&) = default;
	};
}