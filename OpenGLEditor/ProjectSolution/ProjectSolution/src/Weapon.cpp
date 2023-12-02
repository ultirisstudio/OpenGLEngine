#include "Weapon.h"

#include <OpenGLEngine/Script/Script.h>

REGISTER_SCRIPT(Weapon);

void Weapon::OnCreate()
{
	std::cout << "Weapon::OnCreate()" << std::endl;
}

void Weapon::OnDestroy()
{
}

void Weapon::OnUpdate(double dt)
{
}