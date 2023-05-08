#include "frpch.h"
#include "World.h"

World* World::s_Instance = nullptr;

World::World()
{
	//Make sure that there is only one World Instance (Singleton)
	FR_CORE_ASSERT(!s_Instance, "World Already Exists!");
	s_Instance = this;

	//Initialize Chunks
	for (int x = 0; x < WORLD_LENGTH; x++)
	{
		for (int y = 0; y < WORLD_LENGTH; y++)
		{
			m_Chunks[x][y].Init(glm::ivec2(x, y));
		}
	}
}

World::~World()
{
}

void World::Render(Shader* shader)
{
	for (int x = 0; x < WORLD_LENGTH; x++)
	{
		for (int y = 0; y < WORLD_LENGTH; y++)
		{
			m_Chunks[x][y].RenderChunk(shader);
		}
	}
}


char World::GetVoxel(glm::ivec3 worldPos)
{
	//Get the world limit in blocks
	int worldLimitX = CHUNK_LENGTH * WORLD_LENGTH;
	int worldLimitY = CHUNK_HEIGHT;

	//Check for out of bounds
	if (worldPos.x > worldLimitX - 1 || worldPos.x < 0 ||
		worldPos.z > worldLimitX - 1 || worldPos.z < 0 ||
		worldPos.y > worldLimitY - 1 || worldPos.y < 0)
	{
		//return air if we're checking for voxels outside the world
		return Chunk::BLOCK_ID::Air;
	}

	//Find out which chunk the voxel falls,
	glm::ivec2 chunkPos = glm::ivec2(worldPos.x / CHUNK_LENGTH, worldPos.z / CHUNK_LENGTH);

	//Get position relatice to chunk by converting chunk pos to world space first and subtract it from world pos
	glm::ivec3 localPos = worldPos - glm::ivec3(chunkPos.x * CHUNK_LENGTH, 0, chunkPos.y * CHUNK_LENGTH);

	//Check for the voxel
	return m_Chunks[chunkPos.x][chunkPos.y].CheckVoxel(localPos);
}
