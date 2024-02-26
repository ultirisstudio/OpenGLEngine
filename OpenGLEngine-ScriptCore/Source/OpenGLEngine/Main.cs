using System;
using System.Runtime.CompilerServices;
using System.Runtime.Remoting.Messaging;

namespace OpenGLEngine
{
    public struct Vector3
    {
        public float X, Y, Z;

        public Vector3(float x, float y, float z)
        {
            X = x;
            Y = y;
            Z = z;
        }
    }

    public static class InternalCalls
    {
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void NativeLog(string message, int parameter);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void NativeLog_Vector(ref Vector3 parameter, out Vector3 result);
    }

    public class Entity
    {
        public float FloatVar { get; set; }

        public Entity()
        {
            Console.WriteLine("Main !");

            Log("BlaBlaBla", 2002);

            Vector3 pos  = new Vector3(1, 2, 3);
            Vector3 test = Log(pos);
            Console.WriteLine($"test: {test.X}, {test.Y}, {test.Z}");
        }

        public void PrintMessage()
        {
            Console.WriteLine("Hello, World!");
        }

        public void PrintCustomMessage(string message)
        {
            Console.WriteLine($"Print: {message}");
        }

        private void Log(string message, int parameter)
        {
            InternalCalls.NativeLog(message, parameter);
        }

        private Vector3 Log(Vector3 parameter)
        {
            InternalCalls.NativeLog_Vector(ref parameter, out Vector3 result);
            return result;
        }
    }
}