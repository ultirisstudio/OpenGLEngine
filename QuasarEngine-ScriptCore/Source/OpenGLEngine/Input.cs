namespace QuasarEngine
{
    public class Input
    {
        public static bool IsKeyDown(KeyCode keycode)
        {
            return InternalCalls.Input_IsKeyDown(keycode);
        }

        public static bool IsMouseButtonDown(MouseCode key)
        {
            return InternalCalls.Input_IsMouseButtonDown(key);
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
