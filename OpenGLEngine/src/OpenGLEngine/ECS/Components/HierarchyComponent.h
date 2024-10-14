#pragma once

#include <vector>
#include <OpenGLEngine/ECS/Component.h>
#include <OpenGLEngine/Core/UUID.h>

namespace OpenGLEngine
{
	class HierarchyComponent : public Component
	{
	public:
		std::vector<UUID> m_Childrens;
		UUID m_Parent = UUID::Null();

		HierarchyComponent() = default;
		HierarchyComponent(const HierarchyComponent&) = default;

		void AddChild(UUID parent, UUID child);
	};
}