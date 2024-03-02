using System;
using System.Collections.Generic;
using System.Runtime.CompilerServices;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace OpenGLEngine
{
    public static class InternalCalls
    {
        // Debug
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void Debug_Log(string message);
        
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static string[] Debug_ListTest();

        // Entity

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static bool Entity_HasComponent(ulong entityID, Type componentType);

        // Tranform Component

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void TransformComponent_GetTranslation(ulong entityID, out Vector3 translation);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void TransformComponent_SetTranslation(ulong entityID, ref Vector3 translation);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void TransformComponent_GetRotation(ulong entityID, out Vector3 rotation);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void TransformComponent_SetRotation(ulong entityID, ref Vector3 rotation);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void TransformComponent_GetScale(ulong entityID, out Vector3 scale);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void TransformComponent_SetScale(ulong entityID, ref Vector3 scale);

        // Input

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static bool Input_IsKeyDown(KeyCode keycode);
    }
}
