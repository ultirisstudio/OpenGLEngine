#include "qepch.h"

#include <fstream>

#include <GLFW/glfw3.h>

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>

#include <QuasarEngine/Core/Application.h>
#include <QuasarEngine/Renderer/Renderer.h>

#include <QuasarEngine/Scene/Skybox.h>

#include <QuasarEngine/Entity/Entity.h>
#include <QuasarEngine/Resources/Model.h>
#include <QuasarEngine/Resources/Mesh.h>

#include <QuasarEngine/Entity/AllComponents.h>

#include <QuasarEngine/Tools/Math.h>

#include <QuasarEngine/Physic/PhysicEngine.h>
#include <reactphysics3d/reactphysics3d.h>

#include <QuasarEngine/Resources/Debug/DebugLineMesh.h>
#include <QuasarEngine/Resources/Debug/DebugTriangleMesh.h>

namespace QuasarEngine {
	Renderer::SceneData Renderer::m_SceneData = Renderer::SceneData();
	Renderer::DebugRenderData Renderer::m_DebugRenderData = Renderer::DebugRenderData();

	void Renderer::Init()
	{
		RenderCommand::Init();

		ShaderFile shaderFiles;
		shaderFiles.vertexShaderFile = "Assets/Shaders/pbr_shader.vert";
		shaderFiles.fragmentShaderFile = "Assets/Shaders/pbr_shader.frag";

		m_SceneData.m_Shader = Shader::Create(shaderFiles);

		ShaderFile debugTriangleShaderFiles;
		debugTriangleShaderFiles.vertexShaderFile = "Assets/Shaders/debug_triangle.vert";
		debugTriangleShaderFiles.fragmentShaderFile = "Assets/Shaders/debug_triangle.frag";

		m_DebugRenderData.m_DebugTriangleShader = Shader::Create(debugTriangleShaderFiles);

		ShaderFile debugLineShaderFiles;
		debugLineShaderFiles.vertexShaderFile = "Assets/Shaders/debug_line.vert";
		debugLineShaderFiles.fragmentShaderFile = "Assets/Shaders/debug_line.frag";

		m_DebugRenderData.m_DebugLineShader = Shader::Create(debugLineShaderFiles);

		m_SceneData.m_AssetManager = std::make_unique<AssetManager>();
	}

	void Renderer::BeginScene(Scene& scene)
	{
		m_SceneData.m_Scene = &scene;
	}

	void Renderer::Render(BaseCamera& camera)
	{
		glm::mat4 viewMatrix;
		glm::mat4 projectionMatrix;

		viewMatrix = camera.getViewMatrix();
		projectionMatrix = camera.getProjectionMatrix();

		// render degub stuff

		/*std::vector<DebugLineVertex> lines;
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

		m_DebugRenderData.m_DebugLineShader->use();

		m_DebugRenderData.m_DebugLineShader->setUniform("view", viewMatrix);
		m_DebugRenderData.m_DebugLineShader->setUniform("projection", projectionMatrix);

		lineMesh.draw();

		DebugTriangleMesh triangleMesh(triangles);

		m_DebugRenderData.m_DebugTriangleShader->use();

		m_DebugRenderData.m_DebugTriangleShader->setUniform("view", viewMatrix);
		m_DebugRenderData.m_DebugTriangleShader->setUniform("projection", projectionMatrix);

		triangleMesh.draw();

		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);*/

		// Renderer

		m_SceneData.m_Shader->use();
		
		//glActiveTexture(GL_TEXTURE0);
		m_SceneData.m_Scene->getSkybox().BindIrradianceMap(0);
		m_SceneData.m_Shader->setUniform("uIrradianceMap", 0);

		//glActiveTexture(GL_TEXTURE1);
		m_SceneData.m_Scene->getSkybox().BindPrefilterMap(1);
		m_SceneData.m_Shader->setUniform("uPrefilterMap", 1);

		//glActiveTexture(GL_TEXTURE2);
		m_SceneData.m_Scene->getSkybox().BindBrdfLUT(2);
		m_SceneData.m_Shader->setUniform("uBrdfLUT", 2);

		glm::vec3 position;
		glm::decompose(camera.GetTransform(), glm::vec3(), glm::quat(), position, glm::vec3(), glm::vec4());
		
		m_SceneData.m_Shader->setUniform("uCameraPosition", position);

		int dirLightCount = 0;
		int pointLightCount = 0;

		//int totalEntity = 0;
		//int entityDraw = 0;

		for (auto e : m_SceneData.m_Scene->GetAllEntitiesWith<IDComponent>())
		{
			Entity entity{ e, m_SceneData.m_Scene->GetRegistry()};
			m_SceneData.m_Shader->setUniform("uEntity", entity.GetUUID());

			int nat = 3;

			glm::mat4 transform = entity.GetComponent<TransformComponent>().GetGlobalTransform();

			if (entity.HasComponent<LightComponent>())
			{
				auto& lc = entity.GetComponent<LightComponent>();
				if (lc.lightType == LightComponent::LightType::DIRECTIONAL)
				{
					m_SceneData.m_Shader->setUniform("uDirLights[" + std::to_string(dirLightCount) + "].direction", entity.GetComponent<TransformComponent>().Rotation);
					m_SceneData.m_Shader->setUniform("uDirLights[" + std::to_string(dirLightCount) + "].color", lc.dir_color);
					m_SceneData.m_Shader->setUniform("uDirLights[" + std::to_string(dirLightCount) + "].power", lc.dir_power);

					dirLightCount++;
				}
				else if (lc.lightType == LightComponent::LightType::POINT)
				{
					m_SceneData.m_Shader->setUniform("uPointLights[" + std::to_string(pointLightCount) + "].position", entity.GetComponent<TransformComponent>().Position);
					m_SceneData.m_Shader->setUniform("uPointLights[" + std::to_string(pointLightCount) + "].color", lc.point_color);
					m_SceneData.m_Shader->setUniform("uPointLights[" + std::to_string(pointLightCount) + "].attenuation", lc.point_attenuation);
					m_SceneData.m_Shader->setUniform("uPointLights[" + std::to_string(pointLightCount) + "].power", lc.point_power);

					pointLightCount++;
				}
			}

			if (entity.HasComponent<MeshRendererComponent>() && entity.HasComponent<MeshComponent>() && entity.GetComponent<MeshComponent>().HasMesh())
			{
				//totalEntity++;

				if (!entity.GetComponent<MeshRendererComponent>().m_Rendered)
					continue;

				Math::Frustum frustum = Math::CalculateFrustum(camera.getProjectionMatrix() * camera.getViewMatrix());

				if (!entity.GetComponent<MeshComponent>().GetMesh().IsVisible(frustum, transform))
					continue;

				//entityDraw++;

				//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

				m_SceneData.m_Shader->setUniform("uModel", transform);
				m_SceneData.m_Shader->setUniform("uView", viewMatrix);
				m_SceneData.m_Shader->setUniform("uProjection", projectionMatrix);
				m_SceneData.m_Shader->setUniform("uNormalMatrix", glm::transpose(glm::inverse(glm::mat3(transform))));

				Material& material = entity.GetComponent<MaterialComponent>().GetMaterial();

				m_SceneData.m_Shader->setUniform("uMaterial.albedoColor", material.GetAlbedo());
				m_SceneData.m_Shader->setUniform("uMaterial.metallic", material.GetMetallic());
				m_SceneData.m_Shader->setUniform("uMaterial.roughness", material.GetRoughness());
				m_SceneData.m_Shader->setUniform("uMaterial.ao", material.GetAO());

				bool hasAlbedo = material.HasTexture(TextureType::Albedo);
				bool hasNormal = material.HasTexture(TextureType::Normal);
				bool hasMetallic = material.HasTexture(TextureType::Metallic);
				bool hasRoughness = material.HasTexture(TextureType::Roughness);
				bool hasAO = material.HasTexture(TextureType::AO);

				m_SceneData.m_Shader->setUniform("uMaterial.use_albedo_texture", hasAlbedo);
				m_SceneData.m_Shader->setUniform("uMaterial.use_normal_texture", hasNormal);
				m_SceneData.m_Shader->setUniform("uMaterial.use_metallic_texture", hasMetallic);
				m_SceneData.m_Shader->setUniform("uMaterial.use_roughness_texture", hasRoughness);
				m_SceneData.m_Shader->setUniform("uMaterial.use_ao_texture", hasAO);

				if (hasAlbedo)
				{
					//glActiveTexture(GL_TEXTURE0 + nat);
					Texture* albedo = material.GetTexture(TextureType::Albedo);
					if (albedo)
						albedo->Bind(nat);
				}
				m_SceneData.m_Shader->setUniform("uMaterial.albedoMap", nat);
				nat++;

				if (hasNormal)
				{
					//glActiveTexture(GL_TEXTURE0 + nat);
					Texture* normal = material.GetTexture(TextureType::Normal);
					if (normal)
						normal->Bind(nat);
				}
				m_SceneData.m_Shader->setUniform("uMaterial.normalMap", nat);
				nat++;

				if (hasMetallic)
				{
					//glActiveTexture(GL_TEXTURE0 + nat);
					Texture* metallic = material.GetTexture(TextureType::Metallic);
					if (metallic)
						metallic->Bind(nat);
				}
				m_SceneData.m_Shader->setUniform("uMaterial.metallicMap", nat);
				nat++;

				if (hasRoughness)
				{
					//glActiveTexture(GL_TEXTURE0 + nat);
					Texture* roughness = material.GetTexture(TextureType::Roughness);
					if (roughness)
						roughness->Bind(nat);
				}
				m_SceneData.m_Shader->setUniform("uMaterial.roughnessMap", nat);
				nat++;

				if (hasAO)
				{
					//glActiveTexture(GL_TEXTURE0 + nat);
					Texture* ao = material.GetTexture(TextureType::AO);
					if (ao)
						ao->Bind(nat);
				}
				m_SceneData.m_Shader->setUniform("uMaterial.aoMap", nat);
				nat++;

				//glEnable(GL_CULL_FACE);

				entity.GetComponent<MeshComponent>().GetMesh().draw();

				//glDisable(GL_CULL_FACE);

				//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			}

			/*if (entity.HasComponent<TerrainComponent>() && entity.GetComponent<TerrainComponent>().IsGenerated())
			{
				if (entity.GetComponent<TerrainComponent>().m_PolygonMode)
					glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

				entity.GetComponent<TerrainComponent>().GetShader().use();

				entity.GetComponent<TerrainComponent>().GetShader().setUniform("projection", projectionMatrix);
				entity.GetComponent<TerrainComponent>().GetShader().setUniform("view", viewMatrix);
				entity.GetComponent<TerrainComponent>().GetShader().setUniform("model", transform);

				glActiveTexture(GL_TEXTURE0 + nat);
				entity.GetComponent<TerrainComponent>().GetHeightMapTexture().Bind();
				entity.GetComponent<TerrainComponent>().GetShader().setUniform("heightMap", nat);
				nat++;

				glActiveTexture(GL_TEXTURE0 + nat);
				if (entity.GetComponent<MaterialComponent>().GetMaterial().HasTexture(TextureType::Albedo))
					entity.GetComponent<MaterialComponent>().GetMaterial().GetTexture(TextureType::Albedo)->Bind();
				entity.GetComponent<TerrainComponent>().GetShader().setUniform("uTexture", nat);
				nat++;

				entity.GetComponent<TerrainComponent>().GetShader().setUniform("uTextureScale", entity.GetComponent<TerrainComponent>().textureScale);
				entity.GetComponent<TerrainComponent>().GetShader().setUniform("heightMult", entity.GetComponent<TerrainComponent>().heightMult);

				entity.GetComponent<TerrainComponent>().Draw();

				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

				m_SceneData.m_Shader->use();
			}*/
		}

		m_SceneData.m_Shader->setUniform("uUseDirLight", dirLightCount);
		m_SceneData.m_Shader->setUniform("uUsePointLight", pointLightCount);

		//std::cout << entityDraw << "/" << totalEntity << std::endl;

		m_SceneData.m_Shader->unuse();
	}

	void Renderer::RenderSkybox(BaseCamera& camera)
	{
		m_SceneData.m_Scene->getSkybox().DrawSkybox(camera.getViewMatrix(), camera.getProjectionMatrix());
	}

	void Renderer::EndScene()
	{

	}

	void Renderer::LoadModel(const std::string& path)
	{
		std::shared_ptr<Model> model;

		if (!Renderer::m_SceneData.m_AssetManager->isAssetLoaded(path))
		{
			Renderer::m_SceneData.m_AssetManager->loadAsset(path);
		}
		
		model = Renderer::m_SceneData.m_AssetManager->getAsset<Model>(path);

		const size_t slash = path.find_last_of("/\\");
		const std::string m_SelectedFile = path.substr(slash + 1);

		size_t lastindex = m_SelectedFile.find_last_of(".");
		const std::string m_FileName = m_SelectedFile.substr(0, lastindex);

		const std::string m_FolderPath = path.substr(0, slash);

		Entity entity = Renderer::GetScene()->CreateEntity(m_FileName);

		for (auto& [name, mesh] : model->GetMeshes())
		{
			Entity child = Renderer::GetScene()->CreateEntity(name);
			child.AddComponent<MeshRendererComponent>();
			auto& mc = child.AddComponent<MeshComponent>(name, mesh, path);

			//MaterialSpecification material = mc.GetMesh().GetMaterial();
			MaterialSpecification material;

			/*if (material.AlbedoTexture.has_value())
			{
				std::ifstream file(m_FolderPath + "\\" + material.AlbedoTexture.value());
				if (!file.is_open())
				{
					material.AlbedoTexture = std::nullopt;
				}
				else
				{
					material.AlbedoTexture = m_FolderPath + "\\" + material.AlbedoTexture.value();
				}
				file.close();
			}
			
			if (material.NormalTexture.has_value())
			{
				std::ifstream file(m_FolderPath + "\\" + material.NormalTexture.value());
				if (!file.is_open())
				{
					material.AlbedoTexture = std::nullopt;
				}
				else
				{
					material.NormalTexture = m_FolderPath + "\\" + material.NormalTexture.value();
				}
				file.close();
			}*/		

			child.AddComponent<MaterialComponent>(material);

			entity.GetComponent<HierarchyComponent>().AddChild(entity.GetUUID(), child.GetUUID());
		}
	}

	Scene* Renderer::GetScene()
	{
		return m_SceneData.m_Scene;
	}

	double Renderer::GetTime()
	{
		return glfwGetTime();
	}

	RendererAPI::API Renderer::GetAPI()
	{
		return RendererAPI::GetAPI();
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