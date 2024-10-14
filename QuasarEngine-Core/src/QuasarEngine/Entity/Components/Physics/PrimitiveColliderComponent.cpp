#include "qepch.h"
#include "PrimitiveColliderComponent.h"

namespace QuasarEngine
{
	PrimitiveColliderComponent::PrimitiveColliderComponent() : m_Collider(nullptr), mass(1.0f), friction(0.3f), bounciness(0.3f)
	{

	}
}