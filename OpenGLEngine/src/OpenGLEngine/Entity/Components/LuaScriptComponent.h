#pragma once

#include <OpenGLEngine/Entity/Component.h>

#include <include/sol/sol.hpp>

namespace OpenGLEngine
{
	class LuaScriptComponent : public Component
	{
	public:
		sol::protected_function update{ sol::lua_nil }, render{ sol::lua_nil };

		LuaScriptComponent() = default;
		LuaScriptComponent(const LuaScriptComponent&) = default;
		LuaScriptComponent(UUID id) : ID(id) {}
	};
}