#pragma once

#include <OpenGLEngine/Entity/Component.h>
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