#pragma once

#include <string>
#include <OpenGLEngine/ECS/Component.h>

namespace OpenGLEngine
{
	class TagComponent : public Component
	{
	public:
		std::string Tag;

		TagComponent() = default;
		TagComponent(const TagComponent&) = default;
		TagComponent(const std::string& tag) : Tag(tag) {}
	};
}