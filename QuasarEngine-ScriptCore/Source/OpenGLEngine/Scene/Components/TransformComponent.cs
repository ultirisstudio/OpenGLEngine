using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace QuasarEngine
{
    public class TransformComponent : Component
    {
        public Vector3 Translation
        {
            get
            {
                InternalCalls.TransformComponent_GetTranslation(Entity.ID, out Vector3 translation);
                return translation;
            }
            set
            {
                InternalCalls.TransformComponent_SetTranslation(Entity.ID, ref value);
            }
        }

        public Vector3 Rotation
        {
            get
            {
                InternalCalls.TransformComponent_GetRotation(Entity.ID, out Vector3 rotation);
                return rotation;
            }
            set
            {
                InternalCalls.TransformComponent_SetRotation(Entity.ID, ref value);
            }
        }

        public Vector3 Scale
        {
            get
            {
                InternalCalls.TransformComponent_GetScale(Entity.ID, out Vector3 scale);
                return scale;
            }
            set
            {
                InternalCalls.TransformComponent_SetScale(Entity.ID, ref value);
            }
        }

        public Vector3 GetForward()
        {
            InternalCalls.TransformComponent_GetForward(Entity.ID, out Vector3 forward);
            return forward;
        }

        public Vector3 GetRight()
        {
            InternalCalls.TransformComponent_GetRight(Entity.ID, out Vector3 right);
            return right;
        }

        public Vector3 GetTarget()
        {
            InternalCalls.TransformComponent_GetTarget(Entity.ID, out Vector3 target);
            return target;
        }
    }
}
