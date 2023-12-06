#include "depch.h"
#include "ScriptableEntity.h"

#include <OpenGLEngine/Core/Input.h>

namespace OpenGLEngine
{
    bool ScriptableEntity::GetKeyPressed(KeyCode key)
	{
		return Input::IsKeyPressed(key);
	}
}