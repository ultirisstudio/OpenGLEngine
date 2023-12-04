#include "depch.h"
#include "ScriptableEntity.h"

namespace OpenGLEngine
{
    bool ScriptableEntity::GetKeyPressed(KeyCode key)
	{
		return Input::IsKeyPressed(key);
	}
}