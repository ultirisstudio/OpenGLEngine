namespace QuasarEngine
{
    public class CameraComponent : Component
    {
        public void SetFOV(float fov)
        {
            InternalCalls.CameraComponent_SetFOV(Entity.ID, fov);
        }
    }
}
