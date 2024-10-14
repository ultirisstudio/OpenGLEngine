using System;

namespace QuasarEngine
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

        public float GetMapHeight(float x, float z)
        {
            return InternalCalls.Perlin_GetMapHeight(ID, x, z);
        }
    }
}
