using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace OpenGLEngine
{
    public class Player : Entity
    {
        void OnCreate()
        {
            Console.WriteLine($"Player.OnCreate - {ID}");

            Debug.Log("test");

            List<int> list = new List<int> { 1, 2, 3, 4, 5 };

            Debug.Test(list);

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

            Vector3 rotation = Vector3.Zero;

            if (Input.IsKeyDown(KeyCode.J))
                rotation.Z = 1.0f;
            if (Input.IsKeyDown(KeyCode.L))
                rotation.Z = -1.0f;
            if (Input.IsKeyDown(KeyCode.I))
                rotation.X = -1.0f;
            if (Input.IsKeyDown(KeyCode.K))
                rotation.X = 1.0f;

            GetComponent<TransformComponent>().Rotation += rotation * ts;
        }
    }
}
