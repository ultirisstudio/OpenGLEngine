#include "Player.h"

REGISTER_SCRIPT(Player);

void Player::OnCreate()
{
	
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
