#pragma once

#include <QuasarEngine/Entity/Component.h>

#include <sol/sol.hpp>

namespace QuasarEngine
{
	class LuaScriptComponent : public Component
	{
	public:
		sol::protected_function update{ sol::lua_nil }, render{ sol::lua_nil };

		LuaScriptComponent() = default;
		LuaScriptComponent(const LuaScriptComponent&) = default;
	};
}