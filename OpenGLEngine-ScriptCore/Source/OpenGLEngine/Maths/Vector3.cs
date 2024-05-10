using System;

namespace OpenGLEngine
{
    public struct Vector3 : IEquatable<Vector3>
    {
        public float X, Y, Z;

        public static Vector3 Zero => new Vector3(0.0f);

        public Vector3(float value)
        {
            X = Y = Z = value;
        }

        public Vector3(float x, float y, float z)
        {
            X = x;
            Y = y;
            Z = z;
        }

        public static Vector3 operator *(Vector3 vector, float scalar)
        {
            return new Vector3(vector.X * scalar, vector.Y * scalar, vector.Z * scalar);
        }

        public static Vector3 operator +(Vector3 left, Vector3 right)
        {
            return new Vector3(left.X + right.X, left.Y + right.Y, left.Z + right.Z);
        }

        public static Vector3 operator -(Vector3 left, Vector3 right)
        {
            return new Vector3(left.X - right.X, left.Y - right.Y, left.Z - right.Z);
        }

        public bool Equals(Vector3 other)
        {
            return (X == other.X && Y == other.Y && Z == other.Z);
        }

        //Returns a new vector whose elements are the smallest integral values that are greater than or equal to the given vector's elements.
        public static Vector3 Floor(Vector3 vector)
        {
            return new Vector3(FloorComponent(vector.X), FloorComponent(vector.Y), FloorComponent(vector.Z));
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
                    return (int)value; // Cast to an integer if the value is positive
                }
            }
        }

        //returns a new vector whose elements are the smallest integral values that are greater than or equal to the given vector's elements.
        public static Vector3 Ceil(Vector3 vector)
        {
            return new Vector3(CeilComponent(vector.X), CeilComponent(vector.Y), CeilComponent(vector.Z));
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
                    return (int)value;
                }
                else
                {
                    return (int)(value + 1); // Cast to an integer if the value is positive
                }
            }
        }

        public static Vector3 Normalize(Vector3 vector)
        {
            float length = (float)Math.SquareRoot(vector.X * vector.X + vector.Y * vector.Y + vector.Z * vector.Z);
            return new Vector3(vector.X / length, vector.Y / length, vector.Z / length);
        }

        public float Distance(Vector3 vector)
        {
            float dx = vector.X - X;
            float dy = vector.Y - Y;
            float dz = vector.Z - Z;
            return Math.SquareRoot(dx * dx + dy * dy + dz * dz);
        }

        public float Length()
        {
            return Math.SquareRoot(X * X + Y * Y + Z * Z);
        }

    }

}
