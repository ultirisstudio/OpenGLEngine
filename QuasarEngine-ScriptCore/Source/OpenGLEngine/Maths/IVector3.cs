using System;

namespace QuasarEngine
{
    public struct IVector3 : IEquatable<IVector3>
    {
        public int X, Y, Z;

        public static IVector3 Zero => new IVector3(0);

        public IVector3(int value)
        {
            X = Y = Z = value;
        }

        public IVector3(int x, int y, int z)
        {
            X = x;
            Y = y;
            Z = z;
        }

        public static IVector3 operator *(IVector3 vector, int scalar)
        {
            return new IVector3(vector.X * scalar, vector.Y * scalar, vector.Z * scalar);
        }

        public static IVector3 operator +(IVector3 left, IVector3 right)
        {
            return new IVector3(left.X + right.X, left.Y + right.Y, left.Z + right.Z);
        }

        public static IVector3 operator -(IVector3 left, IVector3 right)
        {
            return new IVector3(left.X - right.X, left.Y - right.Y, left.Z - right.Z);
        }

        public bool Equals(IVector3 other)
        {
            return (X == other.X && Y == other.Y && Z == other.Z);
        }
    }
}
