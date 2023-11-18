#include <OpenGLEngine/Entity/ScriptableEntity.h>
#include <OpenGLEngine/Entity/Components/TransformComponent.h>
#include <OpenGLEngine/Core/Input.h>

namespace OpenGLEngine {
	class Player : public ScriptableEntity
	{
	public:
		void OnCreate()
		{

		}

		void OnDestroy()
		{

		}

		void OnUpdate(double dt)
		{
			auto& tc = GetComponent<TransformComponent>();
			float speed = 0.1f;

			//std::cout << tc.Position.x << " " << tc.Position.y << " " << tc.Position.z << std::endl;

			if (Input::IsKeyPressed(Key::A))
				tc.Position.x -= speed * dt;
			if (Input::IsKeyPressed(Key::D))
				tc.Position.x += speed * dt;
			if (Input::IsKeyPressed(Key::W))
				tc.Position.z -= speed * dt;
			if (Input::IsKeyPressed(Key::S))
				tc.Position.z += speed * dt;
		}
	};
}