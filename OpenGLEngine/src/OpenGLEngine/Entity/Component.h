#pragma once

#include <entt.hpp>

namespace OpenGLEngine
{
	class Scene;

	class Component
	{
	public:
		entt::entity entt_entity{ entt::null };
		Scene* scene;

		Component() {}
		virtual ~Component() {}
	};
}