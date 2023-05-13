#pragma once
#include <glm/glm.hpp>
#include "Chunk.h"

//The world currently only has 16 by 16 chunks 


struct Ray
{
	Ray(glm::vec3 origin, glm::vec3 direction)
	{
		Origin = origin;
		Direction = direction;
	}

	glm::vec3 Origin;
	glm::vec3 Direction;
};

struct RayHit
{
	bool IsVoxelHit = false;
	char VoxelId;

	glm::ivec3 VoxelPos;
	glm::ivec3 Normal;
};

// -----------------------------------------------------------------------

class World
{
public:
	//Global Variable for World length
	static const int WORLD_LENGTH = 16;

	World();
	~World();

	void Render(Shader* shader);

	char GetVoxel(glm::ivec3 worldPos);
	void ChangeVoxel(glm::ivec3 worldPos, char voxelID);
	RayHit RayCast(const Ray& ray);

	static World& Get() { return *s_Instance; }

private:
	bool IsChunkInWorld(glm::ivec2 chunkCoord);

private:
	//Create a 16 by 16 chunk array to store our chunks
	Chunk m_Chunks[WORLD_LENGTH][WORLD_LENGTH];
	static World* s_Instance;
};

