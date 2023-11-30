#pragma once

#include "Header.h"

using namespace OpenGLEngine;

class Player : public ScriptableEntity
{
public:
	void OnCreate();

	void OnDestroy();

	void OnUpdate(double dt);
private:
	std::string s_Name = "Player";
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