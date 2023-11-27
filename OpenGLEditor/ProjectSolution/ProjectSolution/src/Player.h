#pragma once

#include "Header.h"

using namespace OpenGLEngine;

class Player : public ScriptableEntity
{
public:
	void OnCreate();

	void OnDestroy();

	void OnUpdate(double dt);

	static bool Init();
private:
	std::string m_Name = "Player";
};

class Test : public ScriptableEntity
{
public:
	void OnCreate();

	void OnDestroy();

	void OnUpdate(double dt);

	static bool Init();
private:
};