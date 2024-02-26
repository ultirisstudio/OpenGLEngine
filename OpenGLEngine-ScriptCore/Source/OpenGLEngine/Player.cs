using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using OpenGLEngine;
using static System.Runtime.CompilerServices.RuntimeHelpers;

namespace Sandbox
{
    public class Player : Entity
    {
        void OnCreate()
        {
            Console.WriteLine($"Player.OnCreate - {ID}");
        }

        void OnUpdate(float ts)
        {
            //Console.WriteLine($"Player.OnUpdate: {ts}");

            float speed = 1.0f;
            Vector3 velocity = Vector3.Zero;

            if (Input.IsKeyDown(KeyCode.Z))
                velocity.Y = 1.0f;
            if (Input.IsKeyDown(KeyCode.S))
                velocity.Y = -1.0f;
            if (Input.IsKeyDown(KeyCode.Q))
                velocity.X = -1.0f;
            if (Input.IsKeyDown(KeyCode.D))
                velocity.X = 1.0f;

            velocity *= speed;

            Vector3 position = Translation;
            position += velocity * ts;
            Translation = position;
        }
    }
}
