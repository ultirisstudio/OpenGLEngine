using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace OpenGLEngine
{
    public struct Mat3x3
    {
        private float[,] mat3x3 = new float[3, 3];

        public Mat3x3(float m00, float m01, float m02, float m10, float m11, float m12, float m20, float m21, float m22)
        {
            mat3x3[0, 0] = m00;
            mat3x3[0, 1] = m01;
            mat3x3[0, 2] = m02;
            mat3x3[1, 0] = m10;
            mat3x3[1, 1] = m11;
            mat3x3[1, 2] = m12;
            mat3x3[2, 0] = m20;
            mat3x3[2, 1] = m21;
            mat3x3[2, 2] = m22;
        }

        public static Mat3x3 Zero => new Mat3x3(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);

        public static Mat3x3 operator *(Mat3x3 mat3x3, Mat3x3 other)
        {
            int numberOfRows = mat3x3.mat3x3.GetLength(0); //retrieve the number of rows in the first matrix
            int numberOfCOlumns = mat3x3.mat3x3.GetLength(1); //retrieve the number of columns in the first matrix
            int numberOfRowsOther = other.mat3x3.GetLength(0); //retrieve the number of rows in the second matrix
            int numberOfColumnsOther = other.mat3x3.GetLength(1); //retrieve the number of columns in the second matrix

            if (numberOfCOlumns != numberOfRowsOther)
            {
                throw new Exception("The number of columns in the first matrix must be equal to the number of rows in the second matrix.");
            }

            return new Mat3x3(
                mat3x3.mat3x3[0, 0] * other.mat3x3[0, 0] + mat3x3.mat3x3[0, 1] * other.mat3x3[1, 0] + mat3x3.mat3x3[0, 2] * other.mat3x3[2, 0], //first row, first column
                mat3x3.mat3x3[0, 0] * other.mat3x3[0, 1] + mat3x3.mat3x3[0, 1] * other.mat3x3[1, 1] + mat3x3.mat3x3[0, 2] * other.mat3x3[2, 1], //first row, second column
                mat3x3.mat3x3[0, 0] * other.mat3x3[0, 2] + mat3x3.mat3x3[0, 1] * other.mat3x3[1, 2] + mat3x3.mat3x3[0, 2] * other.mat3x3[2, 2], //first row, third column
                mat3x3.mat3x3[1, 0] * other.mat3x3[0, 0] + mat3x3.mat3x3[1, 1] * other.mat3x3[1, 0] + mat3x3.mat3x3[1, 2] * other.mat3x3[2, 0], //second row, first column
                mat3x3.mat3x3[1, 0] * other.mat3x3[0, 1] + mat3x3.mat3x3[1, 1] * other.mat3x3[1, 1] + mat3x3.mat3x3[1, 2] * other.mat3x3[2, 1], //second row, second column
                mat3x3.mat3x3[1, 0] * other.mat3x3[0, 2] + mat3x3.mat3x3[1, 1] * other.mat3x3[1, 2] + mat3x3.mat3x3[1, 2] * other.mat3x3[2, 2], //second row, third column
                mat3x3.mat3x3[2, 0] * other.mat3x3[0, 0] + mat3x3.mat3x3[2, 1] * other.mat3x3[1, 0] + mat3x3.mat3x3[2, 2] * other.mat3x3[2, 0], //third row, first column
                mat3x3.mat3x3[2, 0] * other.mat3x3[0, 1] + mat3x3.mat3x3[2, 1] * other.mat3x3[1, 1] + mat3x3.mat3x3[2, 2] * other.mat3x3[2, 1], //third row, second column
                mat3x3.mat3x3[2, 0] * other.mat3x3[0, 2] + mat3x3.mat3x3[2, 1] * other.mat3x3[1, 2] + mat3x3.mat3x3[2, 2] * other.mat3x3[2, 2] //third row, third column
                );
        }

        public static Mat3x3 operator +(Mat3x3 left, Mat3x3 right)
        {
            return new Mat3x3(
                left.mat3x3[0, 0] + right.mat3x3[0, 0], 
                left.mat3x3[0, 1] + right.mat3x3[0, 1], 
                left.mat3x3[0, 2] + right.mat3x3[0, 2], 
                left.mat3x3[1, 0] + right.mat3x3[1, 0], 
                left.mat3x3[1, 1] + right.mat3x3[1, 1], 
                left.mat3x3[1, 2] + right.mat3x3[1, 2], 
                left.mat3x3[2, 0] + right.mat3x3[2, 0], 
                left.mat3x3[2, 1] + right.mat3x3[2, 1], 
                left.mat3x3[2, 2] + right.mat3x3[2, 2]
                );
        }

        public static Mat3x3 operator -(Mat3x3 left, Mat3x3 right)
        {
            return new Mat3x3(
                left.mat3x3[0, 0] - right.mat3x3[0, 0],
                left.mat3x3[0, 1] - right.mat3x3[0, 1],
                left.mat3x3[0, 2] - right.mat3x3[0, 2],
                left.mat3x3[1, 0] - right.mat3x3[1, 0],
                left.mat3x3[1, 1] - right.mat3x3[1, 1],
                left.mat3x3[1, 2] - right.mat3x3[1, 2],
                left.mat3x3[2, 0] - right.mat3x3[2, 0],
                left.mat3x3[2, 1] - right.mat3x3[2, 1],
                left.mat3x3[2, 2] - right.mat3x3[2, 2]
                );
        }

        public bool Equals(Mat3x3 other)
        {
            return (mat3x3[0, 0] == other.mat3x3[0, 0] && mat3x3[0, 1] == other.mat3x3[0, 1] && mat3x3[0, 2] == other.mat3x3[0, 2] && mat3x3[1, 0] == other.mat3x3[1, 0] && mat3x3[1, 1] == other.mat3x3[1, 1] && mat3x3[1, 2] == other.mat3x3[1, 2] && mat3x3[2, 0] == other.mat3x3[2, 0] && mat3x3[2, 1] == other.mat3x3[2, 1] && mat3x3[2, 2] == other.mat3x3[2, 2]);
        }

        public Mat3x3 Rotate(float angle)
        {
            double cos = Math.Cos(angle);
            double sin = Math.Sin(angle);
            return new Mat3x3(
                (int)(mat3x3[0, 0] * cos - mat3x3[0, 1] * sin),
                (int)(mat3x3[0, 0] * sin + mat3x3[0, 1] * cos),
                (int)(mat3x3[0, 2] * cos - mat3x3[0, 2] * sin),
                (int)(mat3x3[1, 0] * cos - mat3x3[1, 1] * sin),
                (int)(mat3x3[1, 0] * sin + mat3x3[1, 1] * cos),
                (int)(mat3x3[1, 2] * cos - mat3x3[1, 2] * sin),
                (int)(mat3x3[2, 0] * cos - mat3x3[2, 1] * sin),
                (int)(mat3x3[2, 0] * sin + mat3x3[2, 1] * cos),
                (int)(mat3x3[2, 2] * cos - mat3x3[2, 2] * sin)
                );
        }
        /* techniquement ça marche mais ça n'a pas trop de sens
        public Mat3x3 MultipliedByVec3(Mat3x3 mat3x3, Vector3 vector3)
        {
            if (vector3.Length() != 3)
            {
                throw new Exception("The number of columns in the matrix must be equal to the number of elements in the vector.");
            }

            return new Mat3x3(
                mat3x3.mat3x3[0, 0] * vector3.X, mat3x3.mat3x3[0, 1] * vector3.Y, mat3x3.mat3x3[0, 2] * vector3.Z,
                mat3x3.mat3x3[1, 0] * vector3.X, mat3x3.mat3x3[1, 1] * vector3.Y, mat3x3.mat3x3[1, 2] * vector3.Z,
                mat3x3.mat3x3[2, 0] * vector3.X, mat3x3.mat3x3[2, 1] * vector3.Y, mat3x3.mat3x3[2, 2] * vector3.Z
                );
        }
        */
        public float[,] MultipliedByVec3 (Mat3x3 mat3X3, Vector3 vector3)
        {
            return new float[3,1]
            {
                { mat3X3.mat3x3[0, 0] * vector3.X + mat3X3.mat3x3[0, 1] * vector3.Y + mat3X3.mat3x3[0, 2] * vector3.Z },
                { mat3X3.mat3x3[1, 0] * vector3.X + mat3X3.mat3x3[1, 1] * vector3.Y + mat3X3.mat3x3[1, 2] * vector3.Z },
                { mat3X3.mat3x3[2, 0] * vector3.X + mat3X3.mat3x3[2, 1] * vector3.Y + mat3X3.mat3x3[2, 2] * vector3.Z }
            };
        }

        public void Scale(float scalar)
        {
            mat3x3[0, 0] *= scalar;
            mat3x3[0, 1] *= scalar;
            mat3x3[0, 2] *= scalar;
            mat3x3[1, 0] *= scalar;
            mat3x3[1, 1] *= scalar;
            mat3x3[1, 2] *= scalar;
            mat3x3[2, 0] *= scalar;
            mat3x3[2, 1] *= scalar;
            mat3x3[2, 2] *= scalar;
        }

    }

}
