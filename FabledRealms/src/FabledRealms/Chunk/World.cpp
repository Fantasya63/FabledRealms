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

	//Generate Mesh for all Chunks
	for (int x = 0; x < WORLD_LENGTH; x++)
	{
		for (int y = 0; y < WORLD_LENGTH; y++)
		{
			m_Chunks[x][y].GenerateMesh();
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


void World::ChangeVoxel(glm::ivec3 worldPos, char voxelID)
{
	//Get the world limit in blocks
	int worldLimitX = CHUNK_LENGTH * WORLD_LENGTH;
	int worldLimitY = CHUNK_HEIGHT;

	//Check for out of bounds
	if (worldPos.x > worldLimitX - 1 || worldPos.x < 0 ||
		worldPos.z > worldLimitX - 1 || worldPos.z < 0 ||
		worldPos.y > worldLimitY - 1 || worldPos.y < 0)
	{
		//We are trying to remove something outside the world
		return;
	}

	//Find out which chunk the voxel falls,
	glm::ivec2 chunkPos = glm::ivec2(worldPos.x / CHUNK_LENGTH, worldPos.z / CHUNK_LENGTH);

	//Get position relatice to chunk by converting chunk pos to world space first and subtract it from world pos
	glm::ivec3 localPos = worldPos - glm::ivec3(chunkPos.x * CHUNK_LENGTH, 0, chunkPos.y * CHUNK_LENGTH);


	//Replace the block to air
	m_Chunks[chunkPos.x][chunkPos.y].SetVoxel(localPos, voxelID);


	//Check if voxel is on the edge of a chunk and Update the neighboring chunks if we are
	//X Axis
	glm::ivec2 neighborChunkPos;
	if (localPos.x == 0)
	{
		//Get neighboring chunk
		neighborChunkPos.x = chunkPos.x - 1;
		neighborChunkPos.y = chunkPos.y;

		//Check if it is on the world
		if (IsChunkInWorld(neighborChunkPos))
		{
			//Update neighboring chunk mesh data to account for the change of voxel data
			m_Chunks[neighborChunkPos.x][neighborChunkPos.y].GenerateMesh();
		}
	}
	else if (localPos.x == CHUNK_LENGTH - 1)
	{
		//Get neighboring chunk
		neighborChunkPos.x = chunkPos.x + 1;
		neighborChunkPos.y = chunkPos.y;

		//Check if it is on the world
		if (IsChunkInWorld(neighborChunkPos))
		{
			//Update neighboring chunk mesh data to account for the change of voxel data
			m_Chunks[neighborChunkPos.x][neighborChunkPos.y].GenerateMesh();
		}
	}

	//Z Axis
	if (localPos.z == 0)
	{
		//Get neighboring chunk
		neighborChunkPos.x = chunkPos.x;
		neighborChunkPos.y = chunkPos.y - 1;

		//Check if it is on the world
		if (IsChunkInWorld(neighborChunkPos))
		{
			//Update neighboring chunk mesh data to account for the change of voxel data
			m_Chunks[neighborChunkPos.x][neighborChunkPos.y].GenerateMesh();
		}
	}
	else if (localPos.z == CHUNK_LENGTH - 1)
	{
		//Get neighboring chunk
		neighborChunkPos.x = chunkPos.x;
		neighborChunkPos.y = chunkPos.y + 1;

		//Check if it is on the world
		if (IsChunkInWorld(neighborChunkPos))
		{
			//Update neighboring chunk mesh data to account for the change of voxel data
			m_Chunks[neighborChunkPos.x][neighborChunkPos.y].GenerateMesh();
		}
	}

	
}


const float MAX_RAY_LENGTH = 10.0;
const int   RAY_STEPS = 50;
const float RAY_STEP_SIZE = MAX_RAY_LENGTH / RAY_STEPS;

RayHit World::RayCast(const Ray& ray)
{
	RayHit hit;

	//march through the world
	//Include the origin of the ray
	char voxel;
	glm::ivec3 voxelPos;
	glm::vec3 pos;

	for (int i = 0; i < RAY_STEPS + 1; i++)
	{
		pos = ray.Origin + ray.Direction * (i * RAY_STEP_SIZE);
		voxelPos = glm::floor(pos);
		voxel = GetVoxel(voxelPos);
		if (voxel == Chunk::BLOCK_ID::Air)
			continue;

		//We hit a block
		hit.IsVoxelHit = true;
		hit.VoxelId = voxel;
		hit.VoxelPos = voxelPos;

		//calculate Hit normal
		glm::vec3 voxelCenter = (glm::vec3)(voxelPos)+glm::vec3(0.5);
		glm::vec3 dir = pos - voxelCenter;
		dir = glm::normalize(dir);

		//Find the axis that's closest to the ray dir
		
		float dotX = glm::abs(glm::dot(dir, glm::vec3(1.0, 0.0, 0.0)));
		float dotY = glm::abs(glm::dot(dir, glm::vec3(0.0, 1.0, 0.0)));
		float dotZ = glm::abs(glm::dot(dir, glm::vec3(0.0, 0.0, 1.0)));

		//Check on X-Axis
		if (dotX > dotY && dotX > dotZ)
		{
			//Check if its in the positive x or negative x
			float x = pos.x > voxelCenter.x ? 1.0 : -1.0;
			hit.Normal = glm::vec3(x, 0.0, 0.0);
		}
		//Check on Y-Axis
		else if (dotY > dotZ)
		{
			//Check if its in the positive y or negative y
			float y = pos.y > voxelCenter.y ? 1.0 : -1.0;
			hit.Normal = glm::vec3(0.0, y, 0.0);
		}
		//Check on Z-Axis
		else
		{
			//Check if its in the positive y or negative y
			float z = pos.z > voxelCenter.z ? 1.0 : -1.0;
			hit.Normal = glm::vec3(0.0, 0.0, z);
		}


		return hit;
	}

	return hit;
}

bool World::IsChunkInWorld(glm::ivec2 chunkCoord)
{
	return (chunkCoord.x > -1 && chunkCoord.x < WORLD_LENGTH  &&
			chunkCoord.y > -1 && chunkCoord.y < WORLD_LENGTH);
}
