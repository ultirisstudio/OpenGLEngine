#include "depch.h"
#include "ScriptGlue.h"

#include <mono/metadata/object.h>

#include <glm/glm.hpp>

namespace OpenGLEngine
{
#define ADD_INTERNAL_CALL(Name) mono_add_internal_call("OpenGLEngine.InternalCalls::" #Name, Name)

	static void NativeLog(MonoString* message, int parameter)
	{
		char* cStr = mono_string_to_utf8(message);
		std::string str(cStr);

		mono_free(cStr);

		std::cout << str << " - " << parameter << std::endl;
	}

	static void NativeLog_Vector(glm::vec3* parameter, glm::vec3* outResult)
	{
		std::cout << "NativeLog_Vector: " << parameter->x << ", " << parameter->y << ", " << parameter->z << std::endl;
		*outResult = *parameter;
	}

	void ScriptGlue::RegisterFunctions()
	{
		ADD_INTERNAL_CALL(NativeLog);
		ADD_INTERNAL_CALL(NativeLog_Vector);
	}
}