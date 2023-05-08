#pragma once
#include "Engine/Graphics/Buffers.h"
#include "Engine/Graphics/VertexArray.h"
#include "Engine/Graphics/Shader.h"

#include <glm/glm.hpp>

#define CHUNK_LENGTH 8
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

	Chunk();
	~Chunk();

	void Init(glm::ivec2 chunkPosition);

	void PopulateVoxelData();
	void GenerateMesh();
	void RenderChunk(Shader* shader);
	char CheckVoxel(glm::ivec3 localPos); //Position relative to the chunk

private:
	//Container for BlockIDS at position (x, y, z)
	char  m_VoxelData[CHUNK_LENGTH][CHUNK_HEIGHT][CHUNK_LENGTH];
	glm::ivec2 m_ChunkPos;

	//Renderer Props
	VertexArray* m_ChunkVAO = nullptr;
	VertexBuffer* m_ChunkVBO = nullptr;
	IndexBuffer* m_ChunkIBO = nullptr;
};
