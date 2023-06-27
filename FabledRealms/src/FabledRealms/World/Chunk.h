#pragma once
#include <Engine/Graphics/Mesh/Mesh.h>
#include <Engine/Graphics/Shader.h>
#include <glm/glm.hpp>

#include <Engine/Graphics/Texture.h>
#include "VoxelData.h"


class Chunk
{
public:
	Chunk();
	~Chunk();

	static const int CHUNK_LENGTH = 16;
	static const int CHUNK_HEIGHT = 256;

	void Init(glm::ivec2 chunkPosition);

	void PopulateVoxelData();
	void GenerateMesh();
	void RenderChunk(Shader* shader);

	char CheckVoxel(glm::ivec3 localPos); //Position relative to the chunk
	void SetVoxel(glm::ivec3 localPos, char voxelID);

public:
	Mesh m_ChunkMesh;

private:
	float CalculateAO(const AOSurroundingData& aoData, glm::ivec3& pos);

private:
	

	//Container for BlockIDS at position (x, y, z)
	char  m_ChunkData[Chunk::CHUNK_LENGTH][Chunk::CHUNK_HEIGHT][Chunk::CHUNK_LENGTH];
	glm::ivec2 m_ChunkPos;

	
};
