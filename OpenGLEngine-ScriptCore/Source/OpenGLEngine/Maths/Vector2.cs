namespace OpenGLEngine
{
    public struct Vector2
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
    }
}
