#include "Player.h"
#include <OpenGLEngine/Script/Script.h>

REGISTER_SCRIPT(Player);

void Player::OnCreate()
{
	std::cout << "Player::OnCreate(): " << m_Name << std::endl;
}

void Player::OnDestroy()
{
}

void Player::OnUpdate(double dt)
{
}

bool Player::Init()
{
	std::cout << "Player::Init()" << std::endl;
	return true;
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

bool Test::Init()
{
	std::cout << "Test::Init()" << std::endl;
	return true;
}