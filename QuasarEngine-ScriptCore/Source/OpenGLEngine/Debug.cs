using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace QuasarEngine
{
    public class Debug
    {
        public static void Log(string message)
        {
            InternalCalls.Debug_Log(message);
        }
    }
}
