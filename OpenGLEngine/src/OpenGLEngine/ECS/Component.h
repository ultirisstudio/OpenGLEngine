#pragma once

#include <entt.hpp>

namespace OpenGLEngine
{
	class Registry;

	class Component
	{
	public:
		entt::entity entt_entity{ entt::null };
		Registry* registry;

		Component() {}
		virtual ~Component() {}
	};
}