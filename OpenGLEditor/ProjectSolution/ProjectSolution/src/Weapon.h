#pragma once

#include <iostream>
#include <OpenGLEngine/Entity/ScriptableEntity.h>

using namespace OpenGLEngine;

class Weapon : public ScriptableEntity
{
public:
	void OnCreate();

	void OnDestroy();

	void OnUpdate(double dt);
};

