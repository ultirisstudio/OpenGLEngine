#include "ChunkManager.h"

#include <QuasarEngine/Renderer/Renderer.h>
#include <QuasarEngine/Entity/Entity.h>
#include <QuasarEngine/Entity/Components/MeshComponent.h>
#include <QuasarEngine/Entity/Components/MaterialComponent.h>
#include <QuasarEngine/Entity/Components/MeshRendererComponent.h>
#include <QuasarEngine/Resources/TextureArray.h>

ChunkManager* ChunkManager::s_Instance = nullptr;

ChunkManager::ChunkManager()
{
	s_Instance = this;

	m_Generator = std::make_unique<TerrainGenerator>();
	//m_Shader = std::make_unique<QuasarEngine::Shader>("Shaders/basic_vertex.glsl", "Shaders/basic_fragment.glsl");

	std::vector<std::string> textures{
		"Assets/Textures/dark_grass_block_top.png",	//1
		"Assets/Textures/dark_grass_block_side.png",	//2
		"Assets/Textures/dark_dirt.png",				//3
		"Assets/Textures/dark_cobblestone.png",		//4
		"Assets/Textures/stone.png",					//5
		"Assets/Textures/log_1.png",					//6
		"Assets/Textures/log_top.png",					//7
		"Assets/Textures/leaves_test.png",				//8
		"Assets/Textures/library_1.png",				//9
		"Assets/Textures/library_2.png",				//10
	};

	// UP, DOWN, LEFT, RIGHT, FRONT, BACK
	m_BlockInfos[BlockType::BLOCK_ERROR] = BlockInfos({ 0, 0, 0, 0, 0, 0 }, true);
	m_BlockInfos[BlockType::AIR] = BlockInfos({ 0, 0, 0, 0, 0, 0 }, true);
	m_BlockInfos[BlockType::GRASS] = BlockInfos({ 1, 1, 2, 2, 2, 2 }, false);
	m_BlockInfos[BlockType::DIRT] = BlockInfos({ 3, 3, 3, 3, 3, 3 }, false);
	m_BlockInfos[BlockType::COBBLE] = BlockInfos({ 4, 4, 4, 4, 4, 4 }, false);
	m_BlockInfos[BlockType::STONE] = BlockInfos({ 5, 5, 5, 5, 5, 5 }, false);
	m_BlockInfos[BlockType::LOG] = BlockInfos({ 7, 7, 6, 6, 6, 6 }, false);
	m_BlockInfos[BlockType::LEAVES] = BlockInfos({ 8, 8, 8, 8, 8, 8 }, true);
	m_BlockInfos[BlockType::LIBRARY_1] = BlockInfos({ 6, 6, 6, 6, 9, 9 }, true);
	m_BlockInfos[BlockType::LIBRARY_2] = BlockInfos({ 6, 6, 6, 6, 10, 10 }, true);


	m_BiomeInfos[BiomeType::MONTAINS] = BiomeInfos(0.8f, 32.0f, 1.0f, 100, -0.2f);
	m_BiomeInfos[BiomeType::PLAINS] = BiomeInfos(0.8f, 32.0f, 1.0f, 100, -0.6f);
	m_BiomeInfos[BiomeType::FOREST] = BiomeInfos(0.6f, 32.0f, 0.8f, 100, 0.0f);
	m_BiomeInfos[BiomeType::DESERT] = BiomeInfos(0.1f, 8.0f, 0.1f, 100, 0.0f);
	m_BiomeInfos[BiomeType::SNOW] = BiomeInfos(0.1f, 8.0f, 0.1f, 100, 0.0f);

	//m_Textures = std::make_unique<TextureArray>(textures);

	QuasarEngine::TextureSpecification spec;
	std::shared_ptr<QuasarEngine::TextureArray> textureArray = QuasarEngine::TextureArray::CreateTextureArray(textures, spec);

	QuasarEngine::Renderer::m_SceneData.m_AssetManager->loadAsset("textures", textureArray);
}

void ChunkManager::SetBlock(const glm::ivec3& position, BlockType voxel)
{
	if (m_EntityMap.find(Math::ToChunkPosition(position)) == m_EntityMap.end())
		return;

	if (!QuasarEngine::Renderer::m_SceneData.m_Scene->GetEntityByUUID(m_EntityMap[Math::ToChunkPosition(position)]).GetComponent<Chunk>().IsMeshGenerated())
		return;

	QuasarEngine::Renderer::m_SceneData.m_Scene->GetEntityByUUID(m_EntityMap[Math::ToChunkPosition(position)]).GetComponent<Chunk>().SetBlock(glm::u8vec3(Math::ToBlockPosition(position)), Block(voxel));
	QuasarEngine::Renderer::m_SceneData.m_Scene->GetEntityByUUID(m_EntityMap[Math::ToChunkPosition(position)]).GetComponent<Chunk>().GenerateMesh();
	QuasarEngine::Renderer::m_SceneData.m_Scene->GetEntityByUUID(m_EntityMap[Math::ToChunkPosition(position)]).GetComponent<Chunk>().ClearMesh();

	if (Math::ToBlockPosition(position).z == 0)
	{
		glm::ivec3 newCoord = position + glm::ivec3(0, 0, -1 * CHUNK_SIZE);
		QuasarEngine::Renderer::m_SceneData.m_Scene->GetEntityByUUID(m_EntityMap[Math::ToChunkPosition(newCoord)]).GetComponent<Chunk>().GenerateMesh();
		QuasarEngine::Renderer::m_SceneData.m_Scene->GetEntityByUUID(m_EntityMap[Math::ToChunkPosition(newCoord)]).GetComponent<Chunk>().ClearMesh();
	}
	else if (Math::ToBlockPosition(position).z == CHUNK_SIZE - 1)
	{
		glm::ivec3 newCoord = position + glm::ivec3(0, 0, 1 * CHUNK_SIZE);
		QuasarEngine::Renderer::m_SceneData.m_Scene->GetEntityByUUID(m_EntityMap[Math::ToChunkPosition(newCoord)]).GetComponent<Chunk>().GenerateMesh();
		QuasarEngine::Renderer::m_SceneData.m_Scene->GetEntityByUUID(m_EntityMap[Math::ToChunkPosition(newCoord)]).GetComponent<Chunk>().ClearMesh();
	}

	if (Math::ToBlockPosition(position).x == 0)
	{
		glm::ivec3 newCoord = position + glm::ivec3(-1 * CHUNK_SIZE, 0, 0);
		QuasarEngine::Renderer::m_SceneData.m_Scene->GetEntityByUUID(m_EntityMap[Math::ToChunkPosition(newCoord)]).GetComponent<Chunk>().GenerateMesh();
		QuasarEngine::Renderer::m_SceneData.m_Scene->GetEntityByUUID(m_EntityMap[Math::ToChunkPosition(newCoord)]).GetComponent<Chunk>().ClearMesh();
	}
	else if (Math::ToBlockPosition(position).x == CHUNK_SIZE - 1)
	{
		glm::ivec3 newCoord = position + glm::ivec3(1 * CHUNK_SIZE, 0, 0);
		QuasarEngine::Renderer::m_SceneData.m_Scene->GetEntityByUUID(m_EntityMap[Math::ToChunkPosition(newCoord)]).GetComponent<Chunk>().GenerateMesh();
		QuasarEngine::Renderer::m_SceneData.m_Scene->GetEntityByUUID(m_EntityMap[Math::ToChunkPosition(newCoord)]).GetComponent<Chunk>().ClearMesh();
	}
}

const Block* ChunkManager::GetBlock(const glm::ivec3& position)
{
	if (m_EntityMap.find(Math::ToChunkPosition(position)) == m_EntityMap.end())
		return nullptr;

	return QuasarEngine::Renderer::m_SceneData.m_Scene->GetEntityByUUID(m_EntityMap[Math::ToChunkPosition(position)]).GetComponent<Chunk>().GetBlock(Math::ToBlockPosition(position));
}

const BlockType ChunkManager::GetBlockType(const glm::ivec3& position)
{
	if (m_EntityMap.find(Math::ToChunkPosition(position)) == m_EntityMap.end())
		return BlockType::BLOCK_ERROR;

	return QuasarEngine::Renderer::m_SceneData.m_Scene->GetEntityByUUID(m_EntityMap[Math::ToChunkPosition(position)]).GetComponent<Chunk>().GetBlockType(Math::ToBlockPosition(position));
}

void ChunkManager::AddChunk(const glm::ivec3& position)
{
	if (m_EntityMap.find(position) == m_EntityMap.end())
	{
		std::string name = std::string("Chunk_" + std::to_string(position.x) + "_" + std::to_string(position.y) + "_" + std::to_string(position.z));
		QuasarEngine::Entity chunk = QuasarEngine::Renderer::m_SceneData.m_Scene->CreateEntity(name);
		chunk.AddComponent<QuasarEngine::MeshComponent>();
		chunk.AddComponent<QuasarEngine::MaterialComponent>();
		chunk.GetComponent<QuasarEngine::MaterialComponent>().GetMaterial().SetTexture(QuasarEngine::TextureType::Albedo, "Assets/Textures/dark_grass_block_top.png");
		chunk.GetComponent<QuasarEngine::MaterialComponent>().GetMaterial().GetRoughness() = 0.0f;
		chunk.GetComponent<QuasarEngine::MaterialComponent>().GetMaterial().GetMetallic() = 0.0f;
		chunk.AddComponent<QuasarEngine::MeshRendererComponent>();
		chunk.AddComponent<Chunk>(position);

		m_EntityMap.emplace(position, chunk.GetUUID());
		QuasarEngine::Renderer::m_SceneData.m_Scene->GetEntityByUUID(m_EntityMap[position]).GetComponent<Chunk>().Generate(*m_Generator);
	}
	else if (QuasarEngine::Renderer::m_SceneData.m_Scene->GetEntityByUUID(m_EntityMap[position]).GetComponent<Chunk>().IsMeshGenerated())
	{
		return;
	}

	for (unsigned i = 0; i < std::size(QuasarEngine::Math::surrounding); i++)
	{
		glm::ivec3 newCoord = position + glm::ivec3(QuasarEngine::Math::surrounding[i].x * CHUNK_SIZE, 0, QuasarEngine::Math::surrounding[i].y * CHUNK_SIZE);
		if (GetChunk(newCoord) == nullptr)
		{
			std::string name = std::string("Chunk_" + std::to_string(newCoord.x) + "_" + std::to_string(newCoord.y) + "_" + std::to_string(newCoord.z));
			QuasarEngine::Entity chunk = QuasarEngine::Renderer::m_SceneData.m_Scene->CreateEntity(name);
			chunk.AddComponent<QuasarEngine::MeshComponent>();
			chunk.AddComponent<QuasarEngine::MaterialComponent>();
			chunk.GetComponent<QuasarEngine::MaterialComponent>().GetMaterial().SetTexture(QuasarEngine::TextureType::Albedo, "Assets/Textures/dark_grass_block_top.png");
			chunk.GetComponent<QuasarEngine::MaterialComponent>().GetMaterial().GetRoughness() = 0.0f;
			chunk.GetComponent<QuasarEngine::MaterialComponent>().GetMaterial().GetMetallic() = 0.0f;
			chunk.AddComponent<QuasarEngine::MeshRendererComponent>();
			chunk.AddComponent<Chunk>(newCoord);

			m_EntityMap.emplace(newCoord, chunk.GetUUID());
			QuasarEngine::Renderer::m_SceneData.m_Scene->GetEntityByUUID(m_EntityMap[newCoord]).GetComponent<Chunk>().Generate(*m_Generator);
		}
	}

	QuasarEngine::Renderer::m_SceneData.m_Scene->GetEntityByUUID(m_EntityMap[position]).GetComponent<Chunk>().GenerateMesh();
	QuasarEngine::Renderer::m_SceneData.m_Scene->GetEntityByUUID(m_EntityMap[position]).GetComponent<Chunk>().ClearMesh();
}

Chunk* ChunkManager::GetChunk(const glm::ivec3& position)
{
	return const_cast<Chunk*>(static_cast<const ChunkManager&>(*this).GetChunk(position));
}

const Chunk* ChunkManager::GetChunk(const glm::ivec3& position) const
{
	auto result = m_EntityMap.find(position);

	if (result == m_EntityMap.end())
		return nullptr;

	return &QuasarEngine::Renderer::m_SceneData.m_Scene->GetEntityByUUID(result->second).GetComponent<Chunk>();
}

int ChunkManager::NeighborCount(glm::ivec3 coord, glm::ivec3 exclude) const
{
	unsigned localCount = 0;
	const Chunk* currentChunk = nullptr;

	auto it = m_EntityMap.begin();

	while (localCount < 4 && it != m_EntityMap.end())
	{
		const glm::ivec3& currentCoord = it->first;

		if (currentCoord == exclude)
		{
			++it;
			continue;
		}

		//currentChunk = it->second.get();
		currentChunk = &QuasarEngine::Renderer::m_SceneData.m_Scene->GetEntityByUUID(it->second).GetComponent<Chunk>();
		if (currentChunk != nullptr && currentChunk->IsMeshGenerated())
			++localCount;

		++it;
	}

	return localCount;
}

void ChunkManager::UpdateChunk(const glm::ivec3& playerPos, float dt)
{
	unsigned loadedChunks = 0;

	glm::ivec3 playerChunkCoord = Math::ToChunkPosition(glm::floor(glm::vec3(playerPos)));
	Chunk* playerChunk = GetChunk(playerChunkCoord);
	bool isPlayerChunkNull = (playerChunk == nullptr);
	bool isPlayerChunkMeshGenerated = (!isPlayerChunkNull && playerChunk->IsMeshGenerated());

	if (isPlayerChunkNull || !isPlayerChunkMeshGenerated)
	{
		loadedChunks++;
		AddChunk(playerChunkCoord);

		for (int i = 0; i < QuasarEngine::Math::DIRECTION_COUNT; i++)
		{
			if (i == QuasarEngine::Math::DIRECTION_UP || i == QuasarEngine::Math::DIRECTION_DOWN)
				continue;

			loadedChunks++;
			AddChunk(playerChunkCoord + glm::ivec3(QuasarEngine::Math::directionVectors[i].x * CHUNK_SIZE, 0, QuasarEngine::Math::directionVectors[i].z * CHUNK_SIZE));
		}
	}

	for (auto it = m_EntityMap.begin(); it != m_EntityMap.end(); ++it)
	{
		Chunk* currentChunk = &QuasarEngine::Renderer::m_SceneData.m_Scene->GetEntityByUUID(it->second).GetComponent<Chunk>();

		currentChunk->UpdateHeightTimer(dt);

		bool isCurrentChunkMeshGenerated = (!(currentChunk == nullptr) && currentChunk->IsMeshGenerated());

		if (ChunkInRange(playerPos, currentChunk->GetPosition()))
		{
			if (loadedChunks < NUMBER_OF_CHUNKS_TO_GENERATE && !isCurrentChunkMeshGenerated && NeighborCount(it->first, it->first) >= 3)
			{
				loadedChunks++;
				AddChunk(it->first);
			}

			currentChunk->SetHeightTimerIncreasing(true);
		}
		else if (isCurrentChunkMeshGenerated)
		{
			currentChunk->SetHeightTimerIncreasing(false);
		}

		if (QuasarEngine::Renderer::m_SceneData.m_Scene->GetEntityByUUID(it->second).GetComponent<Chunk>().HeightTimerHitZero())
		{
			for (unsigned i = 0; i < std::size(QuasarEngine::Math::surrounding); i++)
			{
				glm::ivec3 newCoord = it->first + glm::ivec3(QuasarEngine::Math::surrounding[i].x * CHUNK_SIZE, 0, QuasarEngine::Math::surrounding[i].y * CHUNK_SIZE);
				EntityMap::iterator chunk = m_EntityMap.find(newCoord);

				if (chunk != m_EntityMap.end() && !QuasarEngine::Renderer::m_SceneData.m_Scene->GetEntityByUUID(chunk->second).GetComponent<Chunk>().IsMeshGenerated() && NeighborCount(newCoord, it->first) == 0)
				{
					QuasarEngine::Renderer::m_SceneData.m_Scene->DestroyEntity(QuasarEngine::Renderer::m_SceneData.m_Scene->GetEntityByUUID(chunk->second));
					m_EntityMap.erase(chunk);
				}
			}

			EntityMap::iterator prev = it;
			++it;
			QuasarEngine::Renderer::m_SceneData.m_Scene->DestroyEntity(QuasarEngine::Renderer::m_SceneData.m_Scene->GetEntityByUUID(prev->second));
			m_EntityMap.erase(prev);
		}
	}
}

bool ChunkManager::ChunkInRange(glm::vec3 playerPos, glm::vec3 chunkPos) const
{
	glm::vec3 pos = chunkPos + (glm::vec3(CHUNK_SIZE, 0.0f, CHUNK_SIZE) / 2.f);
	float distanceSquared = glm::distance(glm::vec2(pos.x, pos.z), glm::vec2(playerPos.x, playerPos.z));
	return distanceSquared <= RENDER_DISTANCE * CHUNK_SIZE;
}

BlockInfos& ChunkManager::GetBlockInfos(const BlockType& type)
{
	return m_BlockInfos[type];
}

BiomeInfos& ChunkManager::GetBiomeInfos(const BiomeType& type)
{
	return m_BiomeInfos[type];
}

bool ChunkManager::IsTransparent(const glm::ivec3& position)
{
	return m_BlockInfos[GetBlockType(position)].IsTransparent();
}