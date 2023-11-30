#pragma once

#include <iostream>
#include <string>
#include <OpenGLEngine/Entity/ScriptableEntity.h>

using namespace OpenGLEngine;

class Player : public ScriptableEntity
{
public:
	void OnCreate();

	void OnDestroy();

	void OnUpdate(double dt);
private:
	const char* s_Name = "Player";
	int s_Life = 100;
};

class Test : public ScriptableEntity
{
public:
	void OnCreate();

	void OnDestroy();

	void OnUpdate(double dt);
private:
};