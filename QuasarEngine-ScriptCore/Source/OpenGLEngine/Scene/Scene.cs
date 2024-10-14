using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace QuasarEngine
{
    public class Scene
    {
        public static Entity CreateEntity(string name)
        {
            ulong id = InternalCalls.Scene_CreateEntity(name);
            return new Entity(id);
        }

        public static void RemoveEntity(ulong id)
        {
            InternalCalls.Scene_RemoveEntity(id);
        }
    }
}
