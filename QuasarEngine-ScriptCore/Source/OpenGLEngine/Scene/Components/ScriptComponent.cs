using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace QuasarEngine
{
    public class ScriptComponent : Component
    {
        /*public string ScriptName
        {
            get
            {
                string scriptName = InternalCalls.ScriptComponent_GetScriptName(Entity.ID);
                return scriptName;
            }
            set
            {
                InternalCalls.ScriptComponent_SetScriptName(Entity.ID, ref value);
            }
        }*/

        public void SetScript(string scriptName)
        {
            InternalCalls.ScriptComponent_SetScriptName(Entity.ID, scriptName);
        }
    }
}
