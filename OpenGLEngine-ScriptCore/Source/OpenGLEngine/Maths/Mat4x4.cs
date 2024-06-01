using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace OpenGLEngine
{
    public struct Mat4x4
    {
        private float[,] mat4x4 = new float[4, 4];

        public Mat4x4(float m00, float m01, float m02, float m03, float m10, float m11, float m12, float m13, float m20, float m21, float m22, float m23, float m30, float m31, float m32, float m33)
        {
            mat4x4[0, 0] = m00;
            mat4x4[0, 1] = m01;
            mat4x4[0, 2] = m02;
            mat4x4[0, 3] = m03;
            mat4x4[1, 0] = m10;
            mat4x4[1, 1] = m11;
            mat4x4[1, 2] = m12;
            mat4x4[1, 3] = m13;
            mat4x4[2, 0] = m20;
            mat4x4[2, 1] = m21;
            mat4x4[2, 2] = m22;
            mat4x4[2, 3] = m23;
            mat4x4[3, 0] = m30;
            mat4x4[3, 1] = m31;
            mat4x4[3, 2] = m32;
            mat4x4[3, 3] = m33;
        }

        public static Mat4x4 Zero => new Mat4x4(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);

        public static Mat4x4 operator *(Mat4x4 mat4x4, Mat4x4 other)
        {
            int numberOfRows = mat4x4.mat4x4.GetLength(0); //retrieve the number of rows in the first matrix
            int numberOfCOlumns = mat4x4.mat4x4.GetLength(1); //retrieve the number of columns in the first matrix
            int numberOfRowsOther = other.mat4x4.GetLength(0); //retrieve the number of rows in the second matrix
            int numberOfColumnsOther = other.mat4x4.GetLength(1); //retrieve the number of columns in the second matrix

            if (numberOfCOlumns != numberOfRowsOther)
            {
                throw new Exception("The number of columns in the first matrix must be equal to the number of rows in the second matrix.");
            }

            return new Mat4x4(
                mat4x4.mat4x4[0, 0] * other.mat4x4[0, 0] + mat4x4.mat4x4[0, 1] * other.mat4x4[1, 0] + mat4x4.mat4x4[0, 2] * other.mat4x4[2, 0] + mat4x4.mat4x4[0, 3] * other.mat4x4[3, 0], //first row, first column
                mat4x4.mat4x4[0, 0] * other.mat4x4[0, 1] + mat4x4.mat4x4[0, 1] * other.mat4x4[1, 1] + mat4x4.mat4x4[0, 2] * other.mat4x4[2, 1] + mat4x4.mat4x4[0, 3] * other.mat4x4[3, 1], //first row, second column
                mat4x4.mat4x4[0, 0] * other.mat4x4[0, 2] + mat4x4.mat4x4[0, 1] * other.mat4x4[1, 2] + mat4x4.mat4x4[0, 2] * other.mat4x4[2, 2] + mat4x4.mat4x4[0, 3] * other.mat4x4[3, 2], //first row, third column
                mat4x4.mat4x4[0, 0] * other.mat4x4[0, 3] + mat4x4.mat4x4[0, 1] * other.mat4x4[1, 3] + mat4x4.mat4x4[0, 2] * other.mat4x4[2, 3] + mat4x4.mat4x4[0, 3] * other.mat4x4[3, 3], //first row, fourth column
                mat4x4.mat4x4[1, 0] * other.mat4x4[0, 0] + mat4x4.mat4x4[1, 1] * other.mat4x4[1, 0] + mat4x4.mat4x4[1, 2] * other.mat4x4[2, 0] + mat4x4.mat4x4[1, 3] * other.mat4x4[3, 0], //second row, first column
                mat4x4.mat4x4[1, 0] * other.mat4x4[0, 1] + mat4x4.mat4x4[1, 1] * other.mat4x4[1, 1] + mat4x4.mat4x4[1, 2] * other.mat4x4[2, 1] + mat4x4.mat4x4[1, 3] * other.mat4x4[3, 1], //second row, second column
                mat4x4.mat4x4[1, 0] * other.mat4x4[0, 2] + mat4x4.mat4x4[1, 1] * other.mat4x4[1, 2] + mat4x4.mat4x4[1, 2] * other.mat4x4[2, 2] + mat4x4.mat4x4[1, 3] * other.mat4x4[3, 2], //second row, third column
                mat4x4.mat4x4[1, 0] * other.mat4x4[0, 3] + mat4x4.mat4x4[1, 1] * other.mat4x4[1, 3] + mat4x4.mat4x4[1, 2] * other.mat4x4[2, 3] + mat4x4.mat4x4[1, 3] * other.mat4x4[3, 3], //second row, fourth column
                mat4x4.mat4x4[2, 0] * other.mat4x4[0, 0] + mat4x4.mat4x4[2, 1] * other.mat4x4[1, 0] + mat4x4.mat4x4[2, 2] * other.mat4x4[2, 0] + mat4x4.mat4x4[2, 3] * other.mat4x4[3, 0], //third row, first column
                mat4x4.mat4x4[2, 0] * other.mat4x4[0, 1] + mat4x4.mat4x4[2, 1] * other.mat4x4[1, 1] + mat4x4.mat4x4[2, 2] * other.mat4x4[2, 1] + mat4x4.mat4x4[2, 3] * other.mat4x4[3, 1], //third row, second column
                mat4x4.mat4x4[2, 0] * other.mat4x4[0, 2] + mat4x4.mat4x4[2, 1] * other.mat4x4[1, 2] + mat4x4.mat4x4[2, 2] * other.mat4x4[2, 2] + mat4x4.mat4x4[2, 3] * other.mat4x4[3, 2], //third row, third column
                mat4x4.mat4x4[2, 0] * other.mat4x4[0, 3] + mat4x4.mat4x4[2, 1] * other.mat4x4[1, 3] + mat4x4.mat4x4[2, 2] * other.mat4x4[2, 3] + mat4x4.mat4x4[2, 3] * other.mat4x4[3, 3], //third row, fourth column
                mat4x4.mat4x4[3, 0] * other.mat4x4[0, 0] + mat4x4.mat4x4[3, 1] * other.mat4x4[1, 0] + mat4x4.mat4x4[3, 2] * other.mat4x4[2, 0] + mat4x4.mat4x4[3, 3] * other.mat4x4[3, 0], //fourth row, first column
                mat4x4.mat4x4[3, 0] * other.mat4x4[0, 1] + mat4x4.mat4x4[3, 1] * other.mat4x4[1, 1] + mat4x4.mat4x4[3, 2] * other.mat4x4[2, 1] + mat4x4.mat4x4[3, 3] * other.mat4x4[3, 1], //fourth row, second column
                mat4x4.mat4x4[3, 0] * other.mat4x4[0, 2] + mat4x4.mat4x4[3, 1] * other.mat4x4[1, 2] + mat4x4.mat4x4[3, 2] * other.mat4x4[2, 2] + mat4x4.mat4x4[3, 3] * other.mat4x4[3, 2], //fourth row, third column
                mat4x4.mat4x4[3, 0] * other.mat4x4[0, 3] + mat4x4.mat4x4[3, 1] * other.mat4x4[1, 3] + mat4x4.mat4x4[3, 2] * other.mat4x4[2, 3] + mat4x4.mat4x4[3, 3] * other.mat4x4[3, 3] //fourth row, fourth column
                );
        }

        public static Mat4x4 operator +(Mat4x4 left, Mat4x4 right)
        {
            return new Mat4x4(
                left.mat4x4[0, 0] + right.mat4x4[0, 0],
                left.mat4x4[0, 1] + right.mat4x4[0, 1],
                left.mat4x4[0, 2] + right.mat4x4[0, 2],
                left.mat4x4[0, 3] + right.mat4x4[0, 3],
                left.mat4x4[1, 0] + right.mat4x4[1, 0],
                left.mat4x4[1, 1] + right.mat4x4[1, 1],
                left.mat4x4[1, 2] + right.mat4x4[1, 2],
                left.mat4x4[1, 3] + right.mat4x4[1, 3],
                left.mat4x4[2, 0] + right.mat4x4[2, 0],
                left.mat4x4[2, 1] + right.mat4x4[2, 1],
                left.mat4x4[2, 2] + right.mat4x4[2, 2],
                left.mat4x4[2, 3] + right.mat4x4[2, 3],
                left.mat4x4[3, 0] + right.mat4x4[3, 0],
                left.mat4x4[3, 1] + right.mat4x4[3, 1],
                left.mat4x4[3, 2] + right.mat4x4[3, 2],
                left.mat4x4[3, 3] + right.mat4x4[3, 3]
                );
        }

        public static Mat4x4 operator -(Mat4x4 left, Mat4x4 right)
        {
            return new Mat4x4(
                left.mat4x4[0, 0] - right.mat4x4[0, 0],
                left.mat4x4[0, 1] - right.mat4x4[0, 1],
                left.mat4x4[0, 2] - right.mat4x4[0, 2],
                left.mat4x4[0, 3] - right.mat4x4[0, 3],
                left.mat4x4[1, 0] - right.mat4x4[1, 0],
                left.mat4x4[1, 1] - right.mat4x4[1, 1],
                left.mat4x4[1, 2] - right.mat4x4[1, 2],
                left.mat4x4[1, 3] - right.mat4x4[1, 3],
                left.mat4x4[2, 0] - right.mat4x4[2, 0],
                left.mat4x4[2, 1] - right.mat4x4[2, 1],
                left.mat4x4[2, 2] - right.mat4x4[2, 2],
                left.mat4x4[2, 3] - right.mat4x4[2, 3],
                left.mat4x4[3, 0] - right.mat4x4[3, 0],
                left.mat4x4[3, 1] - right.mat4x4[3, 1],
                left.mat4x4[3, 2] - right.mat4x4[3, 2],
                left.mat4x4[3, 3] - right.mat4x4[3, 3]
                );
        }

        public bool Equals(Mat4x4 other)
        {
            return (mat4x4[0, 0] == other.mat4x4[0, 0] && mat4x4[0, 1] == other.mat4x4[0, 1] && mat4x4[0, 2] == other.mat4x4[0, 2] && mat4x4[0, 3] == other.mat4x4[0, 3] && mat4x4[1, 0] == other.mat4x4[1, 0] && mat4x4[1, 1] == other.mat4x4[1, 1] && mat4x4[1, 2] == other.mat4x4[1, 2] && mat4x4[1, 3] == other.mat4x4[1, 3] && mat4x4[2, 0] == other.mat4x4[2, 0] && mat4x4[2, 1] == other.mat4x4[2, 1] && mat4x4[2, 2] == other.mat4x4[2, 2] && mat4x4[2, 3] == other.mat4x4[2, 3] && mat4x4[3, 0] == other.mat4x4[3, 0] && mat4x4[3, 1] == other.mat4x4[3, 1] && mat4x4[3, 2] == other.mat4x4[3, 2] && mat4x4[3, 3] == other.mat4x4[3, 3]);
        }

        public Mat4x4 Rotate(float angle)
        {
            double cos = Math.Cos(angle);
            double sin = Math.Sin(angle);
            return new Mat4x4(
                (int)(mat4x4[0, 0] * cos - mat4x4[0, 1] * sin),
                (int)(mat4x4[0, 0] * sin + mat4x4[0, 1] * cos),
                (int)(mat4x4[0, 2] * cos - mat4x4[0, 2] * sin),
                (int)(mat4x4[0, 3] * cos - mat4x4[0, 3] * sin),
                (int)(mat4x4[1, 0] * cos - mat4x4[1, 1] * sin),
                (int)(mat4x4[1, 0] * sin + mat4x4[1, 1] * cos),
                (int)(mat4x4[1, 2] * cos - mat4x4[1, 2] * sin),
                (int)(mat4x4[1, 3] * cos - mat4x4[1, 3] * sin),
                (int)(mat4x4[2, 0] * cos - mat4x4[2, 1] * sin),
                (int)(mat4x4[2, 0] * sin + mat4x4[2, 1] * cos),
                (int)(mat4x4[2, 2] * cos - mat4x4[2, 2] * sin),
                (int)(mat4x4[2, 3] * cos - mat4x4[2, 3] * sin),
                (int)(mat4x4[3, 0] * cos - mat4x4[3, 1] * sin),
                (int)(mat4x4[3, 0] * sin + mat4x4[3, 1] * cos),
                (int)(mat4x4[3, 2] * cos - mat4x4[3, 2] * sin),
                (int)(mat4x4[3, 3] * cos - mat4x4[3, 3] * sin)
                );
        }

        public float[,] MultiplyByVec4(float x, float y, float z, float w)
        {
            return new float[4, 1]
            {
                { mat4x4[0, 0] * x + mat4x4[0, 1] * y + mat4x4[0, 2] * z + mat4x4[0, 3] * w },
                { mat4x4[1, 0] * x + mat4x4[1, 1] * y + mat4x4[1, 2] * z + mat4x4[1, 3] * w },
                { mat4x4[2, 0] * x + mat4x4[2, 1] * y + mat4x4[2, 2] * z + mat4x4[2, 3] * w },
                { mat4x4[3, 0] * x + mat4x4[3, 1] * y + mat4x4[3, 2] * z + mat4x4[3, 3] * w }
            };
        }

        public void Scale(float scalar)
        {
            mat4x4[0, 0] *= scalar;
            mat4x4[0, 1] *= scalar;
            mat4x4[0, 2] *= scalar;
            mat4x4[0, 3] *= scalar;
            mat4x4[1, 0] *= scalar;
            mat4x4[1, 1] *= scalar;
            mat4x4[1, 2] *= scalar;
            mat4x4[1, 3] *= scalar;
            mat4x4[2, 0] *= scalar;
            mat4x4[2, 1] *= scalar;
            mat4x4[2, 2] *= scalar;
            mat4x4[2, 3] *= scalar;
            mat4x4[3, 0] *= scalar;
            mat4x4[3, 1] *= scalar;
            mat4x4[3, 2] *= scalar;
            mat4x4[3, 3] *= scalar;
        }
    }
}
