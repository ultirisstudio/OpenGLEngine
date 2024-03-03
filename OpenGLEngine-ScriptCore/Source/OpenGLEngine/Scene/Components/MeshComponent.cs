using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace OpenGLEngine
{
    public class MeshComponent : Component
    {
        public void GenerateMesh(float[] vertices, uint[] indices)
        {
            InternalCalls.MeshComponent_GenerateMesh(Entity.ID, vertices, indices);
        }
    }
}
