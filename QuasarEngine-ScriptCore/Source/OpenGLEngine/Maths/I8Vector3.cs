using System;

namespace QuasarEngine
{
    public struct I8Vector3 : IEquatable<I8Vector3>
    {
        public byte X, Y, Z;

        public static I8Vector3 Zero => new I8Vector3(0);

        public I8Vector3(byte value)
        {
            X = Y = Z = value;
        }

        public I8Vector3(byte x, byte y, byte z)
        {
            X = x;
            Y = y;
            Z = z;
        }

        public static I8Vector3 operator *(I8Vector3 vector, byte scalar)
        {
            return new I8Vector3((byte)(vector.X * scalar), (byte)(vector.Y * scalar), (byte)(vector.Z * scalar));
        }

        public static I8Vector3 operator +(I8Vector3 left, I8Vector3 right)
        {
            return new I8Vector3((byte)(left.X + right.X), (byte)(left.Y + right.Y), (byte)(left.Z + right.Z));
        }

        public static I8Vector3 operator -(I8Vector3 left, I8Vector3 right)
        {
            return new I8Vector3((byte)(left.X - right.X), (byte)(left.Y - right.Y), (byte)(left.Z - right.Z));
        }

        public bool Equals(I8Vector3 other)
        {
            return (X == other.X && Y == other.Y && Z == other.Z);
        }
    }
}
