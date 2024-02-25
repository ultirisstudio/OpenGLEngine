using System;

namespace OpenGLEngine
{
    public class Main
    {
        public float FloatVar { get; set; }

        public Main()
        {
            Console.WriteLine("Main !");
        }

        public void PrintMessage()
        {
            Console.WriteLine("Hello, World!");
        }

        public void PrintCustomMessage(string message)
        {
            Console.WriteLine($"Print: {message}");
        }
    }
}