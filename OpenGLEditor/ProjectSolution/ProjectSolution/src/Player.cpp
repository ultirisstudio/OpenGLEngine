#include "Player.h"
#include <OpenGLEngine/Script/Script.h>
#include <OpenGLEngine/Entity/Components/TransformComponent.h>

REGISTER_SCRIPT(Player);

void Player::OnCreate()
{
	std::cout << "Player::OnCreate(): " << s_Name << std::endl;
}

void Player::OnDestroy()
{
	std::cout << "Player::OnDestroy()" << std::endl;
}

void Player::OnUpdate(double dt)
{
	//TransformComponent& tc = GetComponent<TransformComponent>();
	std::cout << m_Entity.GetName() << std::endl;
}

REGISTER_SCRIPT(Test);

void Test::OnCreate()
{
	std::cout << "Test::OnCreate()" << std::endl;
}

void Test::OnDestroy()
{
}

void Test::OnUpdate(double dt)
{
}