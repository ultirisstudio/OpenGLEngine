using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace OpenGLEngine
{
    public struct Quaternion
    {
        public float X, Y, Z, W;

        public static Quaternion Identity => new Quaternion(0.0f);

        public Quaternion(float value)
        {
            X = Y = Z = W = value;
        }

        public Quaternion(float x, float y, float z, float w)
        {
            X = x;
            Y = y;
            Z = z;
            W = w;
        }

        public static Quaternion Zero => new Quaternion(0.0f);

        public static Quaternion operator *(Quaternion quaternion, float scalar)
        {
            return new Quaternion(quaternion.X * scalar, quaternion.Y * scalar, quaternion.Z * scalar, quaternion.W * scalar);
        }

        public static Quaternion operator +(Quaternion left, Quaternion right)
        {
            return new Quaternion(left.X + right.X, left.Y + right.Y, left.Z + right.Z, left.W + right.W);
        }

        public static Quaternion operator -(Quaternion left, Quaternion right)
        {
            return new Quaternion(left.X - right.X, left.Y - right.Y, left.Z - right.Z, left.W - right.W);
        }

        public bool Equals(Quaternion other)
        {
            return (X == other.X && Y == other.Y && Z == other.Z && W == other.W);
        }

        public static Quaternion Concatenate (Quaternion q1, Quaternion q2)
        {
            float w = q1.W * q2.W - q1.X * q2.X - q1.Y * q2.Y - q1.Z * q2.Z;
            float x = q1.W * q2.X + q1.X * q2.W + q1.Y * q2.Z - q1.Z * q2.Y;
            float y = q1.W * q2.Y - q1.X * q2.Z + q1.Y * q2.W + q1.Z * q2.X;
            float z = q1.W * q2.Z + q1.X * q2.Y - q1.Y * q2.X + q1.Z * q2.W;

            return new Quaternion(w, x, y, z);
        }

        public static Quaternion Inverse(Quaternion quaternion)
        {
            float length = quaternion.X * quaternion.X + quaternion.Y * quaternion.Y + quaternion.Z * quaternion.Z + quaternion.W * quaternion.W;
            float invLength = 1.0f / length;

            return new Quaternion(-quaternion.X * invLength, -quaternion.Y * invLength, -quaternion.Z * invLength, quaternion.W * invLength);
        }

        public static float Length(Quaternion quaternion)
        {
            return (float)Mathf.SquareRoot(quaternion.X * quaternion.X + quaternion.Y * quaternion.Y + quaternion.Z * quaternion.Z + quaternion.W * quaternion.W);
        }



    }
}
