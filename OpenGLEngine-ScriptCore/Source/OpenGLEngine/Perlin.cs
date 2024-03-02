using System;

namespace OpenGLEngine
{
    public class Perlin
    {
        public Perlin()
        {
            ID = InternalCalls.Perlin_Initialize();
        }

        public readonly ulong ID;

        public float GetNoise(float x, float z)
        {
            return InternalCalls.Perlin_GetNoise(ID, x, z);
        }
    }
}
