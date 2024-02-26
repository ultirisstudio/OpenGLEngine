using System;
using OpenGLEngine;

namespace Sandbox
{
    public class Player : Entity
    {
        void OnCreate()
        {
            Console.WriteLine($"Player.OnCreate - {ID}");

            bool hasTransform = HasComponent<TransformComponent>();

            Console.WriteLine("{0}", hasTransform);
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

            GetComponent<TransformComponent>().Translation += velocity * ts;

            //Vector3 position = Translation;
            //position += velocity * ts;
            //Translation = position;
        }
    }
}
