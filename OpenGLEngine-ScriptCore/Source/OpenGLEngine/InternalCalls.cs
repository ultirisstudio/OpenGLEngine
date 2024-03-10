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

        // Scene

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static ulong Scene_CreateEntity(string name);

        // Entity

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static bool Entity_HasComponent(ulong entityID, Type componentType);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void Entity_AddComponent(ulong entityID, Type componentType);

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

        // Mesh Component

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void MeshComponent_GenerateMesh(ulong entityID, float[] vertices, uint[] indices);

        // Perlin

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static ulong Perlin_Initialize();

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static float Perlin_GetNoise(ulong ID, float x, float z);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static float Perlin_GetMapHeight(ulong ID, float x, float z);

        // Script Component

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void ScriptComponent_SetScriptName(ulong ID, string scriptName);

        // RigidBody Component

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void RigidBody_ApplyLocalForceAtCenterOfMass(ulong ID, Vector3 force);

        // Input

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static bool Input_IsKeyDown(KeyCode keycode);
    }
}
