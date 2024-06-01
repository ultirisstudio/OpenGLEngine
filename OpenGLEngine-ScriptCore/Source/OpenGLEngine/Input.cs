namespace OpenGLEngine
{
    public class Input
    {
        public static bool IsKeyDown(KeyCode keycode)
        {
            return InternalCalls.Input_IsKeyDown(keycode);
        }

        public static float GetMouseX()
        {
            return InternalCalls.Input_GetMouseX();
        }

        public static float GetMouseY()
        {
            return InternalCalls.Input_GetMouseY();
        }
    }
}
