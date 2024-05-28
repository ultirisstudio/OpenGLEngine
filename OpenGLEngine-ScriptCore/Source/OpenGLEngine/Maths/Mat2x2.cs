using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace OpenGLEngine
{
     public struct Mat2x2
    {
        private float[,] mat2x2 = new float[2, 2];

        public Mat2x2(float m00, float m01, float m10, float m11)
        {
            mat2x2[0, 0] = m00;
            mat2x2[0, 1] = m01;
            mat2x2[1, 0] = m10;
            mat2x2[1, 1] = m11;
        }

        public static Mat2x2 Zero => new Mat2x2(0.0f, 0.0f, 0.0f, 0.0f);


        public static Mat2x2 operator *(Mat2x2 mat2x2, Mat2x2 other)
        {
            return new Mat2x2(mat2x2.mat2x2[0, 0] * other.mat2x2[0, 0] + mat2x2.mat2x2[0, 1] * other.mat2x2[1, 0], mat2x2.mat2x2[0, 0] * other.mat2x2[0, 1] + mat2x2.mat2x2[0, 1] * other.mat2x2[1, 1], mat2x2.mat2x2[1, 0] * other.mat2x2[0, 0] + mat2x2.mat2x2[1, 1] * other.mat2x2[1, 0], mat2x2.mat2x2[1, 0] * other.mat2x2[0, 1] + mat2x2.mat2x2[1, 1] * other.mat2x2[1, 1]);
        }

        public static Mat2x2 operator +(Mat2x2 left, Mat2x2 right)
        {
            return new Mat2x2(left.mat2x2[0, 0] + right.mat2x2[0, 0], left.mat2x2[0, 1] + right.mat2x2[0, 1], left.mat2x2[1, 0] + right.mat2x2[1, 0], left.mat2x2[1, 1] + right.mat2x2[1, 1]);
        }

        public static Mat2x2 operator -(Mat2x2 left, Mat2x2 right)
        {
            return new Mat2x2(left.mat2x2[0, 0] - right.mat2x2[0, 0], left.mat2x2[0, 1] - right.mat2x2[0, 1], left.mat2x2[1, 0] - right.mat2x2[1, 0], left.mat2x2[1, 1] - right.mat2x2[1, 1]);
        }

        public bool Equals(Mat2x2 other)
        {
            return (mat2x2[0, 0] == other.mat2x2[0, 0] && mat2x2[0, 1] == other.mat2x2[0, 1] && mat2x2[1, 0] == other.mat2x2[1, 0] && mat2x2[1, 1] == other.mat2x2[1, 1]);
        }

        public Mat2x2 Rotate(float angle)
        {
            double cos = Math.Cos(angle);
            double sin = Math.Sin(angle);
            return new Mat2x2((int)(mat2x2[0, 0] * cos - mat2x2[0, 1] * sin), (int)(mat2x2[0, 0] * sin + mat2x2[0, 1] * cos), (int)(mat2x2[1, 0] * cos - mat2x2[1, 1] * sin), (int)(mat2x2[1, 0] * sin + mat2x2[1, 1] * cos));
        }

        /*
        public Mat2x2 MultipliedByVec2 (Mat2x2 mat2x2, Vector2 vector2)
        {
            if (vector2.Length() != 2)
            {
                throw new Exception("The number of columns in the matrix must be equal to the number of elements in the vector.");
            }

            return new Mat2x2(mat2x2.mat2x2[0, 0] * vector2.X , mat2x2.mat2x2[0, 1] * vector2.Z, mat2x2.mat2x2[1, 0] * vector2.X , mat2x2.mat2x2[1, 1] * vector2.Z);
        }
        */

        public float[,] MultiplyByVec2(float x, float y)
        {
            return new float[2, 1] { 
                { mat2x2[0, 0] * x + mat2x2[0, 1] * y }, 
                { mat2x2[1, 0] * x + mat2x2[1, 1] * y } 
            };
        }

        public void Scale(float scalar)
        {
            mat2x2[0, 0] *= scalar;
            mat2x2[0, 1] *= scalar;
            mat2x2[1, 0] *= scalar;
            mat2x2[1, 1] *= scalar;
        }

    }
}
