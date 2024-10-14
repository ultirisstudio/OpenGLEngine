#pragma once

#include <OpenGLEngine/ECS/Component.h>
#include <OpenGLEngine/Core/UUID.h>

namespace OpenGLEngine
{
	class IDComponent : public Component
	{
	public:
		UUID ID;

		IDComponent() = default;
		IDComponent(const IDComponent&) = default;
		IDComponent(UUID id) : ID(id) {}
	};
}