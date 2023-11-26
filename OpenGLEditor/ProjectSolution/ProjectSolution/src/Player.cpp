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