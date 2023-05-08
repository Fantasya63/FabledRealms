#pragma once
#include <glm/glm.hpp>
#include "Chunk.h"

//The world currently only has 16 by 16 chunks 
#define WORLD_LENGTH 16

class World
{
public:
	World();
	~World();

	void Render(Shader* shader);
	char GetVoxel(glm::ivec3 worldPos);
	
	static World& Get() { return *s_Instance; }

private:
	//Create a 16 by 16 chunk array to store our chunks
	Chunk m_Chunks[WORLD_LENGTH][WORLD_LENGTH];

	static World* s_Instance;
};

