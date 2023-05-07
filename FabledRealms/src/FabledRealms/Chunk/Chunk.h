#pragma once

#define CHUNK_WIDTH 8
#define CHUNK_HEIGHT 256

class Chunk
{
public:
	enum BLOCK_ID
	{
		Air = 0,
		Bedrock,
		Stone,
		Dirt,
		Grass,
	};

	void PopulateVoxelData();
	void GenerateMesh();
	void RenderChunk();
	void CheckVoxel(char x, char y, char z);

private:
	//   (x, y, z)
	char  m_VoxelData[CHUNK_WIDTH][CHUNK_HEIGHT][CHUNK_WIDTH];

	//Renderer Props
	uint32_t m_ChunkVAO;
	uint32_t m_ChunkVBO;
};
