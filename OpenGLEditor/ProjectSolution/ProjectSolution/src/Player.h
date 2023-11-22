#pragma once

#include <OpenGLEngine/Entity/ScriptableEntity.h>
#include <OpenGLEngine/Script/Script.h>
//#include <OpenGLEngine/Entity/Components/TransformComponent.h>
//#include <OpenGLEngine/Core/Input.h>

using namespace OpenGLEngine;

class Player;

REGISTER_SCRIPT(Player);

class Player : public ScriptableEntity
{
public:
	void OnCreate()
	{
		std::cout << "create" << std::endl;
	}

	void OnDestroy()
	{

	}

	void OnUpdate(double dt)
	{

	}
private:
};