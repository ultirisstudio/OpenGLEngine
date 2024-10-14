using QuasarEngine;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace QuasarEngine
{
    public class RigidBodyComponent : Component
    {
        public void ApplyLocalForceAtCenterOfMass(Vector3 force)
        {
            InternalCalls.RigidBody_ApplyLocalForceAtCenterOfMass(Entity.ID, force);
        }
    }
}
