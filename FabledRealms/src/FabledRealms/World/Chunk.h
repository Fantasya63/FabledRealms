#pragma once
#include "Engine/Graphics/Buffers.h"
#include "Engine/Graphics/VertexArray.h"
#include "Engine/Graphics/Shader.h"

#include <glm/glm.hpp>
#include "VoxelData.h"


class Chunk
{
public:
	Chunk();
	~Chunk();

	void Init(glm::ivec2 chunkPosition);

	void PopulateVoxelData();
	void GenerateMesh();
	void RenderChunk(Shader* shader);

	char CheckVoxel(glm::ivec3 localPos); //Position relative to the chunk
	void SetVoxel(glm::ivec3 localPos, char voxelID);

private:
	//Container for BlockIDS at position (x, y, z)
	char  m_ChunkData[VoxelData::CHUNK_LENGTH][VoxelData::CHUNK_HEIGHT][VoxelData::CHUNK_LENGTH];
	glm::ivec2 m_ChunkPos;

	//Renderer Props
	VertexArray* m_ChunkVAO = nullptr;
	VertexBuffer* m_ChunkVBO = nullptr;
	IndexBuffer* m_ChunkIBO = nullptr;
};
