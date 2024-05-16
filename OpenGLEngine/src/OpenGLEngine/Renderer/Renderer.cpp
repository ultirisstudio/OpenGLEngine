#include "depch.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>

#include <OpenGLEngine/Core/Application.h>
#include <OpenGLEngine/Renderer/Renderer.h>
#include <OpenGLEngine/Tools/Log.h>

#include <OpenGLEngine/Scene/Skybox.h>

#include <OpenGLEngine/Entity/Components/TransformComponent.h>
#include <OpenGLEngine/Entity/Components/ModelComponent.h>
#include <OpenGLEngine/Entity/Components/MeshComponent.h>
#include <OpenGLEngine/Entity/Components/MaterialComponent.h>
#include <OpenGLEngine/Entity/Components/LightComponent.h>
#include <OpenGLEngine/Entity/Components/TerrainComponent.h>

#include <OpenGLEngine/Tools/Math.h>

#include <OpenGLEngine/Physic/PhysicEngine.h>
#include <reactphysics3d/reactphysics3d.h>

#include <OpenGLEngine/Resources/Debug/DebugLineMesh.h>
#include <OpenGLEngine/Resources/Debug/DebugTriangleMesh.h>

namespace OpenGLEngine {
	Renderer::SceneData Renderer::m_SceneData = Renderer::SceneData();
	Renderer::DebugRenderData Renderer::m_DebugRenderData = Renderer::DebugRenderData();

	void Renderer::Init()
	{
		m_SceneData.m_Shader = Shader();
		m_SceneData.m_Shader.LoadFromFile("Shaders/pbr_shader.vert", "Shaders/pbr_shader.frag");

		m_DebugRenderData.m_DebugTriangleShader = Shader();
		m_DebugRenderData.m_DebugTriangleShader.LoadFromFile("Shaders/debug_triangle.vert", "Shaders/debug_triangle.frag");

		m_DebugRenderData.m_DebugLineShader = Shader();
		m_DebugRenderData.m_DebugLineShader.LoadFromFile("Shaders/debug_line.vert", "Shaders/debug_line.frag");
	}

	void Renderer::BeginScene(Scene& scene)
	{
		m_SceneData.m_Scene = &scene;
	}

	void Renderer::Render(BaseCamera& camera)
	{
		// view and projection matrices

		glm::mat4 viewMatrix;
		glm::mat4 projectionMatrix;

		viewMatrix = camera.getViewMatrix();
		projectionMatrix = camera.getProjectionMatrix();

		// render degub stuff

		std::vector<DebugLineVertex> lines;
		std::vector<DebugTriangleVertex> triangles;

		for (const reactphysics3d::DebugRenderer::DebugLine& line : PhysicEngine::GetDebugRenderer().getLines())
		{
			DebugLineVertex lineVertex_1;
			DebugLineVertex lineVertex_2;

			lineVertex_1.position = glm::vec3(line.point1.x, line.point1.y, line.point1.z);
			lineVertex_1.color = m_DebugRenderData.GetColorFromUint_32t(line.color1);

			lineVertex_2.position = glm::vec3(line.point2.x, line.point2.y, line.point2.z);
			lineVertex_2.color = m_DebugRenderData.GetColorFromUint_32t(line.color1);

			lines.push_back(lineVertex_1);
			lines.push_back(lineVertex_2);
		}

		for (const reactphysics3d::DebugRenderer::DebugTriangle& triangle : PhysicEngine::GetDebugRenderer().getTriangles())
		{
			DebugTriangleVertex triangleVertex_1;
			DebugTriangleVertex triangleVertex_2;
			DebugTriangleVertex triangleVertex_3;

			triangleVertex_1.position = glm::vec3(triangle.point1.x, triangle.point1.y, triangle.point1.z);
			triangleVertex_1.color = m_DebugRenderData.GetColorFromUint_32t(triangle.color1);

			triangleVertex_2.position = glm::vec3(triangle.point2.x, triangle.point2.y, triangle.point2.z);
			triangleVertex_2.color = m_DebugRenderData.GetColorFromUint_32t(triangle.color2);

			triangleVertex_3.position = glm::vec3(triangle.point3.x, triangle.point3.y, triangle.point3.z);
			triangleVertex_3.color = m_DebugRenderData.GetColorFromUint_32t(triangle.color3);

			triangles.push_back(triangleVertex_1);
			triangles.push_back(triangleVertex_2);
			triangles.push_back(triangleVertex_3);
		}

		//Render lines and triangles
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		DebugLineMesh lineMesh(lines);

		m_DebugRenderData.m_DebugLineShader.use();

		m_DebugRenderData.m_DebugLineShader.setUniform("view", viewMatrix);
		m_DebugRenderData.m_DebugLineShader.setUniform("projection", projectionMatrix);

		lineMesh.draw();

		DebugTriangleMesh triangleMesh(triangles);

		m_DebugRenderData.m_DebugTriangleShader.use();

		m_DebugRenderData.m_DebugTriangleShader.setUniform("view", viewMatrix);
		m_DebugRenderData.m_DebugTriangleShader.setUniform("projection", projectionMatrix);

		triangleMesh.draw();

		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		// Renderer

		m_SceneData.m_Shader.use();

		glm::vec3 position, rotation, scale;
		Math::DecomposeTransform(camera.GetTransform(), position, rotation, scale);

		m_SceneData.m_Shader.setUniform("uCameraPosition", position);

		int dirLightCount = 0;
		int pointLightCount = 0;

		for (auto entity = m_SceneData.m_Scene->getEntities()->begin(); entity != m_SceneData.m_Scene->getEntities()->end(); entity++)
		{
			m_SceneData.m_Shader.use();
			m_SceneData.m_Shader.setUniform("uEntity", entity->second.GetUUID());

			int nat = 0;

			glm::mat4& transform = entity->second.GetComponent<TransformComponent>().GetTransform();

			UUID parentID = entity->second.m_Parent;
			while (parentID != UUID::Null())
			{
				Entity* parent = &m_SceneData.m_Scene->getEntities()->find(parentID)->second;
				if (parent != nullptr)
				{
					glm::mat4& parentTransform = parent->GetComponent<TransformComponent>().GetTransform();
					glm::vec3 parentPosition, parentScale;
					glm::quat parentRotationQuat;
					glm::decompose(parentTransform, parentScale, parentRotationQuat, parentPosition, glm::vec3(), glm::vec4());
					glm::vec3 rotation = glm::eulerAngles(parentRotationQuat);
					
					transform = glm::translate(transform, parentPosition);
					//transform = glm::rotate(transform, rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
					//transform = glm::rotate(transform, rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
					//transform = glm::rotate(transform, rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));

					transform = glm::scale(transform, parentScale);

					parentID = parent->m_Parent;
				}
			}

			if (entity->second.HasComponent<LightComponent>())
			{
				auto& lc = entity->second.GetComponent<LightComponent>();
				if (lc.lightType == LightComponent::LightType::DIRECTIONAL)
				{
					m_SceneData.m_Shader.setUniform("uDirLights[" + std::to_string(dirLightCount) + "].direction", entity->second.GetComponent<TransformComponent>().Rotation);
					m_SceneData.m_Shader.setUniform("uDirLights[" + std::to_string(dirLightCount) + "].color", lc.dir_color);

					dirLightCount++;
				}
				else if (lc.lightType == LightComponent::LightType::POINT)
				{
					m_SceneData.m_Shader.setUniform("uPointLights[" + std::to_string(pointLightCount) + "].position", entity->second.GetComponent<TransformComponent>().Position);
					m_SceneData.m_Shader.setUniform("uPointLights[" + std::to_string(pointLightCount) + "].color", lc.point_color);
					m_SceneData.m_Shader.setUniform("uPointLights[" + std::to_string(pointLightCount) + "].attenuation", lc.point_attenuation);
					m_SceneData.m_Shader.setUniform("uPointLights[" + std::to_string(pointLightCount) + "].power", lc.point_power);

					pointLightCount++;
				}
			}

			if (entity->second.HasComponent<MeshComponent>() && entity->second.GetComponent<MeshComponent>().HasMesh())
			{
				//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

				m_SceneData.m_Shader.use();

				Material& material = entity->second.GetComponent<MaterialComponent>().GetMaterial();

				m_SceneData.m_Shader.setUniform("uModel", transform);
				m_SceneData.m_Shader.setUniform("uView", viewMatrix);
				m_SceneData.m_Shader.setUniform("uProjection", projectionMatrix);
				m_SceneData.m_Shader.setUniform("uNormalMatrix", glm::transpose(glm::inverse(glm::mat3(transform))));

				m_SceneData.m_Shader.setUniform("uMaterial.albedoColor", *material.getVec3("albedo"));
				m_SceneData.m_Shader.setUniform("uMaterial.metallic", *material.getFloat("metallic"));
				m_SceneData.m_Shader.setUniform("uMaterial.roughness", *material.getFloat("roughness"));
				m_SceneData.m_Shader.setUniform("uMaterial.ao", *material.getFloat("ao"));

				m_SceneData.m_Shader.setUniform("uMaterial.use_albedo_texture", static_cast<bool>(*material.getBoolean("albedo")));
				m_SceneData.m_Shader.setUniform("uMaterial.use_normal_texture", static_cast<bool>(*material.getBoolean("normal")));
				m_SceneData.m_Shader.setUniform("uMaterial.use_metallic_texture", static_cast<bool>(*material.getBoolean("metallic")));
				m_SceneData.m_Shader.setUniform("uMaterial.use_roughness_texture", static_cast<bool>(*material.getBoolean("roughness")));
				m_SceneData.m_Shader.setUniform("uMaterial.use_ao_texture", static_cast<bool>(*material.getBoolean("ao")));

				if (*material.getBoolean("albedo"))
				{
					glActiveTexture(GL_TEXTURE0 + nat);
					material.getTexture("albedo")->Bind();
					m_SceneData.m_Shader.setUniform("uMaterial.albedoMap", nat);
					nat++;
				}

				if (*material.getBoolean("normal"))
				{
					glActiveTexture(GL_TEXTURE0 + nat);
					material.getTexture("normal")->Bind();
					m_SceneData.m_Shader.setUniform("uMaterial.normalMap", nat);
					nat++;
				}

				if (*material.getBoolean("metallic"))
				{
					glActiveTexture(GL_TEXTURE0 + nat);
					material.getTexture("metallic")->Bind();
					m_SceneData.m_Shader.setUniform("uMaterial.metallicMap", nat);
					nat++;
				}

				if (*material.getBoolean("roughness"))
				{
					glActiveTexture(GL_TEXTURE0 + nat);
					material.getTexture("roughness")->Bind();
					m_SceneData.m_Shader.setUniform("uMaterial.roughnessMap", nat);
					nat++;
				}

				if (*material.getBoolean("ao"))
				{
					glActiveTexture(GL_TEXTURE0 + nat);
					material.getTexture("ao")->Bind();
					m_SceneData.m_Shader.setUniform("uMaterial.aoMap", nat);
					nat++;
				}

				glActiveTexture(GL_TEXTURE0 + nat);
				m_SceneData.m_Scene->getSkybox().BindIrradianceMap();
				m_SceneData.m_Shader.setUniform("uIrradianceMap", nat);
				nat++;

				entity->second.GetComponent<MeshComponent>().GetMesh().draw();

				//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			}

			if (entity->second.HasComponent<TerrainComponent>() && entity->second.GetComponent<TerrainComponent>().IsGenerated())
			{
				if (entity->second.GetComponent<TerrainComponent>().m_PolygonMode)
					glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

				entity->second.GetComponent<TerrainComponent>().GetShader().use();

				entity->second.GetComponent<TerrainComponent>().GetShader().setUniform("projection", projectionMatrix);
				entity->second.GetComponent<TerrainComponent>().GetShader().setUniform("view", viewMatrix);
				entity->second.GetComponent<TerrainComponent>().GetShader().setUniform("model", transform);

				glActiveTexture(GL_TEXTURE0 + nat);
				entity->second.GetComponent<TerrainComponent>().GetHeightMapTexture().Bind();
				entity->second.GetComponent<TerrainComponent>().GetShader().setUniform("heightMap", nat);
				nat++;

				glActiveTexture(GL_TEXTURE0 + nat);
				entity->second.GetComponent<MaterialComponent>().GetMaterial().getTexture("albedo")->Bind();
				entity->second.GetComponent<TerrainComponent>().GetShader().setUniform("uTexture", nat);
				nat++;

				entity->second.GetComponent<TerrainComponent>().GetShader().setUniform("uTextureScale", entity->second.GetComponent<TerrainComponent>().textureScale);
				entity->second.GetComponent<TerrainComponent>().GetShader().setUniform("heightMult", entity->second.GetComponent<TerrainComponent>().heightMult);

				entity->second.GetComponent<TerrainComponent>().Draw();

				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

				m_SceneData.m_Shader.use();
			}
		}

		m_SceneData.m_Shader.setUniform("uUseDirLight", dirLightCount);
		m_SceneData.m_Shader.setUniform("uUsePointLight", pointLightCount);
		m_SceneData.m_Shader.setUniform("uAmbiantLight", m_SceneData.m_Scene->m_AmbientLight);
	}

	void Renderer::RenderSkybox(BaseCamera& camera)
	{
		glm::mat4 viewMatrix;
		glm::mat4 projectionMatrix;

		viewMatrix = camera.getViewMatrix();
		projectionMatrix = camera.getProjectionMatrix();

		m_SceneData.m_Scene->getSkybox().GetShader()->use();
		m_SceneData.m_Scene->getSkybox().GetShader()->setUniform("projection", projectionMatrix);
		m_SceneData.m_Scene->getSkybox().GetShader()->setUniform("view", viewMatrix);

		glActiveTexture(GL_TEXTURE0);
		m_SceneData.m_Scene->getSkybox().BindCubeMap();
		m_SceneData.m_Scene->getSkybox().GetModel()->draw();
	}

	void Renderer::EndScene()
	{

	}

	void Renderer::Clear() {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void Renderer::ClearColor(const glm::vec4& color) {
		glClearColor(color.r, color.g, color.b, color.a);
	}

	void Renderer::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
	{
		glViewport(x, y, width, height);
	}

	Scene* Renderer::GetScene()
	{
		return m_SceneData.m_Scene;
	}

	double Renderer::GetTime()
	{
		return glfwGetTime();
	}

	const glm::vec3& Renderer::DebugRenderData::GetColorFromUint_32t(uint32_t color)
	{
		unsigned char rouge = (color >> 24) & 0xFF;
		unsigned char vert = (color >> 16) & 0xFF;
		unsigned char bleu = (color >> 8) & 0xFF;

		float rougeNormalise = rouge / 255.0f;
		float vertNormalise = vert / 255.0f;
		float bleuNormalise = bleu / 255.0f;

		return glm::vec3(rougeNormalise, vertNormalise, bleuNormalise);
	}
}