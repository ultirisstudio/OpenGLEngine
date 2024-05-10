using System;

namespace OpenGLEngine
{
    public struct Vector2 : IEquatable<Vector2>
    {
        public float X, Z;

        public static Vector2 Zero => new Vector2(0.0f);

        public Vector2(float value)
        {
            X = Z = value;
        }

        public Vector2(float x, float z)
        {
            X = x;
            Z = z;
        }

        public static Vector2 operator *(Vector2 vector, float scalar)
        {
            return new Vector2(vector.X * scalar, vector.Z * scalar);
        }

        public static Vector2 operator +(Vector2 left, Vector2 right)
        {
            return new Vector2(left.X + right.X, left.Z + right.Z);
        }

        public static Vector2 operator -(Vector2 left, Vector2 right)
        {
            return new Vector2(left.X - right.X, left.Z - right.Z);
        }

        public bool Equals(Vector2 other)
        {
            return (X == other.X && Z == other.Z);
        }

        //Returns a new vector whose elements are the smallest integral values that are greater than or equal to the given vector's elements.
        public static Vector2 Floor(Vector2 vector)
        {
            return new Vector2(FloorComponent(vector.X), FloorComponent(vector.Z));
        }

        private static float FloorComponent(float value)
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
                    return (int)value; // Cast to an integer if the value is positive
                }
            }
        }

        //returns a new vector whose elements are the smallest integral values that are greater than or equal to the given vector's elements.
        public static Vector2 Ceiling(Vector2 vector)
        {
            return new Vector2(CeilingComponent(vector.X), CeilingComponent(vector.Z));
        }

        private static float CeilingComponent(float value)
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
                    return (int)value; // Simply cast to an integer if the value is negative
                }
                else
                {
                    return (int)(value + 1); // Add 1 before casting to an integer if the value is positive
                }
            }
        }

        public static Vector2 Normalize (Vector2 vector)
        {
            float length = (float)Math.SquareRoot(vector.X * vector.X + vector.Z * vector.Z);
            return new Vector2(vector.X / length, vector.Z / length);
        }

        public float Distance(Vector2 vector)
        {
            float dx = vector.X - X;
            float dz = vector.Z - Z;
            return Math.SquareRoot(dx * dx + dz * dz);
        }

        public float Length()
        {
            return Math.SquareRoot(X * X + Z * Z);
        }

    }

}
