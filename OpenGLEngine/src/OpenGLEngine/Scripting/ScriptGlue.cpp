#include "depch.h"
#include "ScriptGlue.h"

#include <mono/metadata/object.h>

#include <glm/glm.hpp>

#include <OpenGLEngine/Scene/Scene.h>
#include <OpenGLEngine/Entity/Entity.h>
#include <OpenGLEngine/Scripting/ScriptEngine.h>
#include <OpenGLEngine/Entity/Components/TransformComponent.h>

#include <OpenGLEngine/Tools/UUID.h>
#include <OpenGLEngine/Core/KeyCodes.h>
#include <OpenGLEngine/Core/Input.h>

namespace OpenGLEngine
{
#define ADD_INTERNAL_CALL(Name) mono_add_internal_call("OpenGLEngine.InternalCalls::" #Name, Name)

	static void Entity_GetTranslation(UUID entityID, glm::vec3* outTranslation)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		Entity* entity = scene->GetEntityByUUID(entityID);
		*outTranslation = entity->GetComponent<TransformComponent>().Position;
	}

	static void Entity_SetTranslation(UUID entityID, glm::vec3* translation)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		Entity* entity = scene->GetEntityByUUID(entityID);
		entity->GetComponent<TransformComponent>().Position = *translation;
	}

	static bool Input_IsKeyDown(KeyCode keycode)
	{
		return Input::IsKeyPressed(keycode);
	}

	void ScriptGlue::RegisterFunctions()
	{
		ADD_INTERNAL_CALL(Entity_GetTranslation);
		ADD_INTERNAL_CALL(Entity_SetTranslation);

		ADD_INTERNAL_CALL(Input_IsKeyDown);
	}
}