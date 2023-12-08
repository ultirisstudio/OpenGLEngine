#include "Player.h"

#include <iostream>
#include <OpenGLEngine/Script/Script.h>
#include <OpenGLEngine/Core/KeyCodes.h>
#include <OpenGLEngine/Entity/Components/TransformComponent.h>

REGISTER_SCRIPT(Player);

void Player::OnCreate()
{
	std::cout << "Player::OnCreate(): " << s_Name << " - life:" << s_Life << std::endl;
}

void Player::OnDestroy()
{
	std::cout << "Player::OnDestroy()" << std::endl;
}

void Player::OnUpdate(double dt)
{
	TransformComponent& tc = GetComponent<TransformComponent>();
	//tc.Position.x += 0.02f * dt;
	//std::cout << m_Entity.GetName() << std::endl;
	
	if (GetKeyPressed(Key::A))
	{
		std::cout << "W" << std::endl;
	}
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

REGISTER_SCRIPT(Inventory);

void Inventory::OnCreate()
{
	std::cout << "Test::OnCreate()" << std::endl;
}

void Inventory::OnDestroy()
{
}

void Inventory::OnUpdate(double dt)
{
}