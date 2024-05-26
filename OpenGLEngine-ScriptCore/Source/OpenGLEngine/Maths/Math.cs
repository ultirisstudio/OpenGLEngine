using System;
using System.Collections.Generic;
using System.Linq;
using System.Security.Policy;
using System.Text;
using System.Threading.Tasks;

namespace OpenGLEngine
{
    public class Math
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

        public static float Cos(float x)
        {
            float result = 1;
            float term = 1;
            for (int i = 1; i < 10; i++)
            {
                term *= -x * x / (2 * i * (2 * i - 1));
                result += term;
            }
            return result;
        }

        public static float Sin(float x)
        {
            float result = x;
            float term = x;
            for (int i = 1; i < 10; i++)
            {
                term *= -x * x / (2 * i * (2 * i + 1));
                result += term;
            }
            return result;
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

       
    }
}
