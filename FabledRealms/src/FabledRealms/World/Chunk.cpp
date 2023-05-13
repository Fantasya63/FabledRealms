#include "frpch.h"
#include <string>
#include <iostream>
#include <filesystem>
#include <fstream>

#include "Chunk.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glad/glad.h>
#include <FastNoiseLite.h>

#include <vector>
#include "World.h"


Chunk::Chunk()
{
}

Chunk::~Chunk()
{
	//Save Data to disk
	std::string saveDir = "game/data/world/";
	std::string filename = std::to_string(m_ChunkPos.x) + "-" + std::to_string(m_ChunkPos.y) + ".bin";
	
	//Create a directory if it doesnt exist already
	std::filesystem::create_directory(saveDir);

	//Create and write file for this chunk
	std::ofstream file(saveDir + filename, std::ios::binary);

	//Write Data
	file.write(reinterpret_cast<const char*>(m_ChunkData), sizeof(m_ChunkData));
	
	//close the file
	file.close();

	DLOG_CORE_INFO("Saving Chunks");

	delete m_ChunkVAO;
	delete m_ChunkVBO;
	delete m_ChunkIBO;
}

void Chunk::Init(glm::ivec2 chunkPosition)
{
	//Set the coordinates for the chunk
	m_ChunkPos = chunkPosition;

	//Check for Chunk file
	std::string filePath = "game/data/world/" + std::to_string(m_ChunkPos.x) + "-" + std::to_string(m_ChunkPos.y) + ".bin";

	std::ifstream saveFile;
	saveFile.open(filePath, std::ios::binary);

	//If we succesfully opened the file
	if (saveFile.is_open())
	{
		//Read saved Data
		saveFile.read(reinterpret_cast<char*>(m_ChunkData), sizeof(m_ChunkData));
	}
	else
	{
		//We Generate data
		PopulateVoxelData();
	}
	
}

void Chunk::SetVoxel(glm::ivec3 localPos, char voxelID)
{
	m_ChunkData[localPos.x][localPos.y][localPos.z] = voxelID;
	GenerateMesh();
}

char Chunk::CheckVoxel(glm::ivec3 localPos)
{
	return m_ChunkData[localPos.x][localPos.y][localPos.z];
}

void Chunk::PopulateVoxelData()
{
	//Terrain Generation

	// Create and configure FastNoise object
	FastNoiseLite noise;
	noise.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
	noise.SetFractalType(FastNoiseLite::FractalType_FBm);
	noise.SetFractalOctaves(2);

	//Heighmap Data
	float Heightmap[16][16];

	for (int x = 0; x < VoxelData::CHUNK_LENGTH; x++)
	{
		for (int z = 0; z < VoxelData::CHUNK_LENGTH; z++)
		{
			//Get Heightmap data from procedural noise
			//Get worldspace coordinates for this position
			glm::ivec2 world = (m_ChunkPos * glm::ivec2(World::WORLD_LENGTH)) + glm::ivec2(x, z);

			//Get Heightmap data
			Heightmap[x][z] = noise.GetNoise(static_cast<float>(world.x), static_cast<float>(world.y));

			for (int y = 0; y < VoxelData::CHUNK_HEIGHT; y++)
			{
				

				int groundLevel = 50 + Heightmap[x][z] * 10.0;

				//Set the bottom of the world to be Bedrock
				if (y == 0)
				{
					m_ChunkData[x][y][z] = VoxelData::BLOCK_ID::Bedrock;
					continue;
				}

				//Anything below ground level - 3 is stone
				if (y < groundLevel - 3)
				{
					m_ChunkData[x][y][z] = VoxelData::BLOCK_ID::Stone;
					continue;
				}


				//Y level below groundlevel are dirt
				if (y < groundLevel)
				{
					m_ChunkData[x][y][z] = VoxelData::BLOCK_ID::Dirt;
					continue;
				}

				//At ground Level are grass
				if (y == groundLevel)
				{
					m_ChunkData[x][y][z] = VoxelData::BLOCK_ID::Grass;
					continue;
				}

				//Anything else are air
				m_ChunkData[x][y][z] = VoxelData::BLOCK_ID::Air;
			}
		}
	}
}

void Chunk::GenerateMesh()
{
	//Check if there is already a mesh generated for this chunk
	if (m_ChunkVAO != nullptr)
	{
		//Delete the mesh
		delete m_ChunkVAO;
		delete m_ChunkVBO;
		delete m_ChunkIBO;

		m_ChunkVAO = nullptr;
		m_ChunkVBO = nullptr;
		m_ChunkIBO = nullptr;
	}


	std::vector<float> vertices;
	// Reserve 8192 floats ideal for a flat world 
	// (16 * 16 * 4 * 8) Chunk area * Vertices per face * num Floats per vertices (Position, Normal, UV)
	vertices.reserve(8192);

	std::vector<uint32_t> indices;
	// Reserve 8192 floats ideal for a flat world 
	// (16 * 16 * 6) Chunk area * Num of Indices per face
	indices.reserve(1536);
		

	//For keeping track of the index values
	uint32_t currentIndex = 0;

	//Loop throught every voxel in the chunk
	for (int x = 0; x < VoxelData::CHUNK_LENGTH; x++)
	{
		for (int y = 0; y < VoxelData::CHUNK_HEIGHT; y++)
		{
			for (int z = 0; z < VoxelData::CHUNK_LENGTH; z++)
			{
				//Position of this voxel
				glm::ivec3 voxelPos = glm::ivec3(x, y, z);

				//Get the voxel at position (x, y, z)
				char voxelID = m_ChunkData[x][y][z];
				
				//Air doesnt neeed to be rendered
				if (voxelID == VoxelData::BLOCK_ID::Air)
					continue;

				//Check Surrounding Blocks
				char temp;
				for (int face = 0; face < VoxelData::TOTAL_NUM_OF_CUBE_FACES; face++)
				{
					glm::ivec3 normal = VoxelData::CUBE_FACE_NORMALS[face];
					
					//Check if there's a block infront of the face
					glm::ivec3 checkPos = voxelPos + normal;
					//Convert the position to world Space
					checkPos += glm::vec3(m_ChunkPos.x, 0.0, m_ChunkPos.y) * glm::vec3(VoxelData::CHUNK_LENGTH);

					char blockCheckID = World::Get().GetVoxel(checkPos);
					
					//If the face has a block in front of it, then it is occluded so we dont need to render what we cant see
					if (blockCheckID != VoxelData::BLOCK_ID::Air)
						continue;

					int uvIndex = VoxelData::VOXEL_UV_INDICES[voxelID][face];

					//Loop through all of the face's vertices
					for (int vert = 0; vert < VoxelData::TOTAL_NUMBER_OF_CUBE_VERTS; vert++)
					{
						//get vertex position at this voxel pos
						glm::ivec3 pos = voxelPos + VoxelData::CUBE_FACE_VERTICES_POS[face][vert];

						//Add x, y, and z positions
						vertices.emplace_back(pos.x);
						vertices.emplace_back(pos.y);
						vertices.emplace_back(pos.z);

						//Add Normals
						vertices.emplace_back(normal.x);
						vertices.emplace_back(normal.y);
						vertices.emplace_back(normal.z);



						//Get the UV from the lookup table
						glm::vec2 uv = VoxelData::GetUVSFromUVIndex(uvIndex, vert);

						//Add UVs
						vertices.emplace_back(uv.x);
						vertices.emplace_back(uv.y);
					}

					// Add Indices
					// These are the indices of the vertex that make up a Face
					// Each face has two triangles to make a square
					
					// First Triangle
					indices.emplace_back(currentIndex);    
					indices.emplace_back(currentIndex + 1);
					indices.emplace_back(currentIndex + 2);

					// Second Triangle
					indices.emplace_back(currentIndex);
					indices.emplace_back(currentIndex + 2);
					indices.emplace_back(currentIndex + 3);

					// Increment current index by 4.
					// as there are 4 vertices per face
					currentIndex += 4;
				}
			}
		}
	}

	// SetUp Vertex Array
	m_ChunkVAO = VertexArray::Create();
	m_ChunkVAO->Bind();

	// SetUp Vertex Buffer
	//Takes in a float* to the array and the size of the array in bytes
	m_ChunkVBO = VertexBuffer::Create(&vertices[0], vertices.size() * sizeof(float));
	
	// SetUp Index Buffer
	// Takes in a uint32_t* as the array and the number of elements the array has
	m_ChunkIBO = IndexBuffer::Create(&indices[0], indices.size());
}

void Chunk::RenderChunk(Shader* shader)
{
	// Set Model Matrix
	// This is the position, rotation, and scale of the Chunk
	// In this case, we're only moving the chunk bby a certain amount
	glm::vec3 pos = glm::vec3(m_ChunkPos.x * VoxelData::CHUNK_LENGTH, 0.0f, m_ChunkPos.y * VoxelData::CHUNK_LENGTH);
	glm::mat4 modelMatrix = glm::mat4(1.0); //Initialize to Identity matrix
	modelMatrix = glm::translate(modelMatrix, pos);

	shader->SetMat4("a_ModelMatrix", modelMatrix);

	//Bind the Vertex Array that's about to be rendered
	m_ChunkVAO->Bind();
	
	//Render the VAO
	glDrawElements(GL_TRIANGLES, m_ChunkIBO->GetCount(), GL_UNSIGNED_INT, 0);
}