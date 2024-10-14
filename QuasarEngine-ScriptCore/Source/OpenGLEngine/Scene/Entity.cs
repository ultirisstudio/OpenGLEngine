using System;
using System.Collections.Generic;
using System.Runtime.CompilerServices;

namespace QuasarEngine
{
    public class Entity
    {
        protected Entity() { ID = 0; }

        internal Entity(ulong id)
        {
            ID = id;
        }

        public readonly ulong ID;

        public bool HasComponent<T>() where T : Component, new()
        {
            Type componentType = typeof(T);
            return InternalCalls.Entity_HasComponent(ID, componentType);
        }

        public T AddComponent<T>() where T : Component, new()
        {
            if (HasComponent<T>())
                return null;

            Type componentType = typeof(T);
            InternalCalls.Entity_AddComponent(ID, componentType);

            T component = new T() { Entity = this };
            return component;
        }

        public T GetComponent<T>() where T : Component, new()
        {
            if (!HasComponent<T>())
                return null;

            T component = new T() { Entity = this };
            return component;
        }

        public Entity GetChildByName(string name)
        {
            ulong id = InternalCalls.Entity_GetChildByName(ID, name);
            return new Entity(id);
        }
    }
}