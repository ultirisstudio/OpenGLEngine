#pragma once
#include <iostream>
#include <OpenGLEngine/Script/Script.h>

using namespace OpenGLEngine;

class Player : public ScriptableEntity
{
public:
	void OnCreate();

	void OnDestroy();

	void OnUpdate(double dt);

	static bool Init();
private:
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