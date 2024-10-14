namespace QuasarEngine
{
    public class CharacterControllerComponent : Component
    {
        public void Move(Vector3 force)
        {
            InternalCalls.CharacterController_Move(Entity.ID, force);
        }
    }
}