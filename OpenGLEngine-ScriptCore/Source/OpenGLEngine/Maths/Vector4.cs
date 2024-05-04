using System;

namespace OpenGLEngine
{
    public struct Vector4 : IEquatable<Vector4>
    {
        public float X, Y, Z, W;

        public static Vector4 Zero => new Vector4(0.0f);

        public Vector4(float value)
        {
            X = Y = Z = W = value;
        }

        public Vector4(float x, float y, float z, float w)
        {
            X = x;
            Y = y;
            Z = z;
            W = w;
        }

        public static Vector4 operator *(Vector4 vector, float scalar)
        {
            return new Vector4(vector.X * scalar, vector.Y * scalar, vector.Z * scalar, vector.W * scalar);
        }

        public static Vector4 operator +(Vector4 left, Vector4 right)
        {
            return new Vector4(left.X + right.X, left.Y + right.Y, left.Z + right.Z, left.W + right.W);
        }

        public static Vector4 operator -(Vector4 left, Vector4 right)
        {
            return new Vector4(left.X - right.X, left.Y - right.Y, left.Z - right.Z, left.W - right.W);
        }

        public bool Equals(Vector4 other)
        {
            return (X == other.X && Y == other.Y && Z == other.Z && W == other.W);
        }

        //Returns a new vector whose elements are the smallest integral values that are greater than or equal to the given vector's elements.
        public static Vector4 Floor(Vector4 vector)
        {
            return new Vector4(FloorComponent(vector.X), FloorComponent(vector.Y), FloorComponent(vector.Z), FloorComponent(vector.W));
        }

        public static float FloorComponent(float value)
        {
            // Check if the value is already an integer
            if (value % 1 == 0)
            {
                return value; // Return the value unchanged
            }
            else
            {
                // If the value is negative, subtract 1 before casting to an integer
                if (value < 0)
                {
                    return (int)(value - 1);
                }
                else
                {
                    return (int)value;
                }
            }
        }

        public static Vector4 Ceil(Vector4 vector)
        {
            return new Vector4(CeilComponent(vector.X), CeilComponent(vector.Y), CeilComponent(vector.Z), CeilComponent(vector.W));
        }

        public static float CeilComponent(float value)
        {
            // Check if the value is already an integer
            if (value % 1 == 0)
            {
                return value; // Return the value unchanged
            }
            else
            {
                // If the value is negative, subtract 1 before casting to an integer
                if (value < 0)
                {
                    return (int)(value - 1);
                }
                else
                {
                    return (int)value + 1; // Cast to an integer if the value is positive
                }
            }
        }

        public static Vector4 Normalize(Vector4 value)
        {
            float length = (float)Math.SquareRoot(value.X * value.X + value.Y * value.Y + value.Z * value.Z + value.W * value.W);
            return new Vector4(value.X / length, value.Y / length, value.Z / length, value.W / length);
        }

        public float Distance(Vector4 value)
        {
            return (float)Math.SquareRoot((X - value.X) * (X - value.X) + (Y - value.Y) * (Y - value.Y) + (Z - value.Z) * (Z - value.Z) + (W - value.W) * (W - value.W));
        }

        public float Length()
        {
            return (float)Math.SquareRoot(X * X + Y * Y + Z * Z + W * W);
        }

    }
}
