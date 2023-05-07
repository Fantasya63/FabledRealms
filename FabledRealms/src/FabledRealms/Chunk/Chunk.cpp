#include "frpch.h"
#include "Chunk.h"

#include <vector>

void Chunk::PopulateVoxelData()
{
	for (int x = 0; x < CHUNK_WIDTH; x++)
	{
		for (int y = 0; y < CHUNK_HEIGHT; y++)
		{
			for (int z = 0; z < CHUNK_WIDTH; z++) 
			{
				//Terrain Generation

				//Set the bottom of the world to be Bedrock
				if (y == 0)
				{
					m_VoxelData[x][y][z] = BLOCK_ID::Bedrock;
					continue;
				}

				//Anything below y level 47 is stone
				if (y < 47)
				{
					m_VoxelData[x][y][z] = BLOCK_ID::Stone;
					continue;
				}


				//Y level 47-49 are dirt
				if (y < 50)
				{
					m_VoxelData[x][y][z] = BLOCK_ID::Dirt;
					continue;
				}

				//At Y level 50 are Grass
				if (y == 50)
				{
					m_VoxelData[x][y][z] = BLOCK_ID::Grass;
					continue;
				}

				//Anything else are air
				m_VoxelData[x][y][z] = BLOCK_ID::Air;
			}
		}
	}
}

void Chunk::GenerateMesh()
{
	std::vector<float> vertices;

	for (int x = 0; x < CHUNK_WIDTH; x++)
	{
		for (int y = 0; y < CHUNK_HEIGHT; y++)
		{
			for (int z = 0; z < CHUNK_WIDTH; z++)
			{
				char voxel = m_VoxelData[x][y][z];
				
				//Air doesnt neeed to be rendered
				if (voxel == BLOCK_ID::Air)
					continue;

				
			}
		}
	}
}

void Chunk::RenderChunk()
{
}
