using System;
using System.Collections.Generic;
using System.Linq;
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


    }
}
