namespace OpenGLEngine
{
    public struct RaycastInfo
    {
        public bool Hit;
        public float Distance;
        public Vector3 HitPoint;
        public Vector3 Normal;
        public ulong Entity;
    }

    public class Physics
    {
        public static RaycastInfo RaycastAll(Vector3 origin, Vector3 direction, float distance)
        {
            InternalCalls.Physics_RaycastAll(origin, direction, distance, out RaycastInfo raycastInfos);
            return raycastInfos;
        }
    }
}
