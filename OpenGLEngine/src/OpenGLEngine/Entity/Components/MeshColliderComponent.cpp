#include "depch.h"
#include "MeshColliderComponent.h"

#include <reactphysics3d/reactphysics3d.h>

#include <OpenGLEngine/Entity/Entity.h>
#include <OpenGLEngine/Physic/PhysicEngine.h>
#include <OpenGLEngine/Entity/Components/TransformComponent.h>
#include <OpenGLEngine/Entity/Components/MeshComponent.h>
#include <OpenGLEngine/Entity/Components/RigidBodyComponent.h>

namespace OpenGLEngine
{
	MeshColliderComponent::MeshColliderComponent()
	{
        
	}

    void MeshColliderComponent::Init()
    {
        auto& mc = entity->GetComponent<MeshComponent>();

        // Create the TriangleVertexArray
        const int nbVertices = mc.GetMesh().GetVerticesCount();
        const int nbTriangles = mc.GetMesh().GetIndicesCount() / 3;

        float* vertices = new float[nbVertices];
        for (int i = 0; i < nbVertices; i++)
        {
			vertices[i] = mc.GetMesh().GetVertices()[i].position.x;
			vertices[i] = mc.GetMesh().GetVertices()[i].position.y;
			vertices[i] = mc.GetMesh().GetVertices()[i].position.z;
		}

        int* indices = new int[nbTriangles];
        std::copy(mc.GetMesh().GetIndices().begin(), mc.GetMesh().GetIndices().end(), indices);

        triangleVertexArray =
            new reactphysics3d::TriangleVertexArray(nbVertices, vertices, 3 * sizeof(float), nbTriangles,
                indices, 3 * sizeof(int),
                reactphysics3d::TriangleVertexArray::VertexDataType::VERTEX_FLOAT_TYPE,
                reactphysics3d::TriangleVertexArray::IndexDataType::INDEX_INTEGER_TYPE);

        // Create the TriangleMesh
        std::vector<rp3d::Message> messages;
        reactphysics3d::TriangleMesh* triangleMesh = PhysicEngine::GetPhysicsCommon()->createTriangleMesh(*triangleVertexArray, messages);

        // Display the messages (info, warning and errors)
        if (messages.size() > 0) {

            for (const rp3d::Message& message : messages) {

                std::string messageType;
                switch (message.type) {
                case rp3d::Message::Type::Information:
                    messageType = "info";
                    break;
                case rp3d::Message::Type::Warning:
                    messageType = "warning";
                    break;
                case rp3d::Message::Type::Error:
                    messageType = "error";
                    break;
                }

                std::cout << "Message (" << messageType << "): " << message.text << std::endl;
            }
        }

        // Make sure there was no errors during mesh creation
        assert(triangleMesh != nullptr);

        const reactphysics3d::Vector3 scaling(1, 1, 1);

        // Create the ConcaveMeshShape using the TriangleMesh
        reactphysics3d::ConcaveMeshShape* concaveMeshShape = PhysicEngine::GetPhysicsCommon()->createConcaveMeshShape(triangleMesh, scaling);

        if (entity->HasComponent<RigidBodyComponent>())
        {
            auto& rb = entity->GetComponent<RigidBodyComponent>();
            rb.GetRigidBody()->addCollider(concaveMeshShape, reactphysics3d::Transform::identity());
        }
    }
}