#pragma once

#include <entt/entt.hpp>

namespace OpenGLEngine
{
	class Scene;

	class Component
	{
	public:
		entt::entity e{ entt::null };
		Scene* s;

		Component() {}
		virtual ~Component() {}
	};
}