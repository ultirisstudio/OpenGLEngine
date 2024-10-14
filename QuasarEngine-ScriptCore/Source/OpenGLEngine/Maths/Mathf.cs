using System;

namespace QuasarEngine
{
    public class Mathf
    {
        public static float SquareRoot(float x)
        {
            float guess = x / 2.0f;
            float lastGuess;

            do
            {
                lastGuess = guess;
                guess = (guess + x / guess) / 2.0f;
            } while (lastGuess != guess);

            return guess;
        }

        public static float abs(float x)
        {
            return x < 0 ? -x : x;
        }

        public static float Cos(double angle)
        {
            angle = angle % (2 * Math.PI);

            double term = 1;
            double sum = term;

            for (int i = 1; i <= 10; i++)
            {
                term *= -angle * angle / (2 * i * (2 * i - 1));
                sum += term;
            }

            return (float)sum;
        }
        public static float Sin(double angle)
        {
            angle = angle % (2 * Math.PI);

            double term = angle;
            double sum = term;

            for (int i = 1; i <= 10; i++)
            {
                term *= -angle * angle / ((2 * i) * (2 * i + 1));
                sum += term;
            }

            return (float)sum;
        }

        public static float Tan(float x)
        {
            return Sin(x) / Cos(x);
        }

        public static float Pow(float x, float y)
        {
            float result = 1;
            for (int i = 0; i < y; i++)
            {
                result *= x;
            }
            return result;
        }

        public static float exp(float x)
        {
            float result = 1;
            float term = 1;
            for (int i = 1; i < 10; i++)
            {
                term *= x / i;
                result += term;
            }
            return result;
        }

        public static float lerp(float start_value, float end_value, float t)
        {
            return start_value + t * (end_value - start_value);
        }

        public static float clamp(float value, float min, float max)
        {
            return value < min ? min : value > max ? max : value;
        }

        public static Vector3 Cross(Vector3 a, Vector3 b)
        {
            return new Vector3(a.Y * b.Z - a.Z * b.Y, a.Z * b.X - a.X * b.Z, a.X * b.Y - a.Y * b.X);
        }

        public static float Radians(float degrees)
        {
            const float PI = (float)Math.PI;
            return degrees * (PI / 180.0f);
        }

        public static float Degrees(float radians)
        {
            const float degreesPerRadian = (float)(180.0f / Math.PI);
            return radians * degreesPerRadian;
        }
    }
}
