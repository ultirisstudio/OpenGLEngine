#pragma once

#include <QuasarEngine/Entity/Components/Physics/PrimitiveColliderComponent.h>

#include <glm/glm.hpp>

namespace QuasarEngine
{
	class BoxColliderComponent : public PrimitiveColliderComponent
	{
	public:
		BoxColliderComponent();
		~BoxColliderComponent();

		void Init() override;
		void UpdateColliderMaterial() override;
		void UpdateColliderSize() override;

		bool m_UseEntityScale = true;
		glm::vec3 m_Size = { 1.0f, 1.0f, 1.0f };
	};
}