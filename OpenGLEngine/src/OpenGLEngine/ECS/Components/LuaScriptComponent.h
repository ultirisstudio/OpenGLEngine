#pragma once

#include <OpenGLEngine/ECS/Component.h>

#include <sol/sol.hpp>

namespace OpenGLEngine
{
	class LuaScriptComponent : public Component
	{
	public:
		sol::protected_function update{ sol::lua_nil }, render{ sol::lua_nil };

		LuaScriptComponent() = default;
		LuaScriptComponent(const LuaScriptComponent&) = default;
	};
}