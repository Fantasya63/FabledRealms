#include "frpch.h"
#include <string>
#include <fstream>

#include "Chunk.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glad/glad.h>

#include <vector>
#include "World.h"

#define CUBE_FACES 6
#define NUM_CUBE_FACE_VERTS 4

// UVs for each block with index corresponding to Block ID.
// Each block has 4 posible UV Coordinates
// Values are calculated from the terrains's texture atlas (blocks.png)
const glm::vec2 BLOCK_UVS[][4]
{
	// Air, Air has no UV cause we're not rendering them anyways
	{
		glm::vec2(0.0f, 0.0f), // Top Left
		glm::vec2(0.0f, 0.0f), // Bottom Left
		glm::vec2(0.0f, 0.0f), // Bottom Rightqa
		glm::vec2(0.0f, 0.0f), // Top Right
	},

	// Bedrock,
	{
		glm::vec2(0.0f, 1.0f), // Top Left
		glm::vec2(0.0f, 0.5f), // Bottom Left
		glm::vec2(0.5f, 0.5f), // Bottom Right
		glm::vec2(0.5f, 1.0f), // Top Right
	},

	// Stone,
	{
		glm::vec2(0.5f, 0.5f), // Top Left
		glm::vec2(0.5f, 0.0f), // Bottom Left
		glm::vec2(1.0f, 0.0f), // Bottom Right
		glm::vec2(1.0f, 0.5f), // Top Right
	},

	// Dirt,
	{
		glm::vec2(0.5f, 1.0f), // Top Left
		glm::vec2(0.5f, 0.5f), // Bottom Left
		glm::vec2(1.0f, 0.5f), // Bottom Right
		glm::vec2(1.0f, 1.0f), // Top Right
	},

	// Grass,
	{
		glm::vec2(0.0f, 0.5f), // Top Left
		glm::vec2(0.0f, 0.0f), // Bottom Left
		glm::vec2(0.5f, 0.0f), // Bottom Right
		glm::vec2(0.5f, 0.5f), // Top Right
	},
};

const glm::ivec3 CUBE_FACE_NORMALS[]
{
	glm::ivec3( 0.0,  0.0, -1.0), //FRONT
	glm::ivec3( 0.0,  0.0,  1.0), //BACK
	glm::ivec3(-1.0,  0.0,  0.0), //LEFT
	glm::ivec3( 1.0,  0.0,  0.0), //RIGHT
	glm::ivec3( 0.0,  1.0,  0.0), //TOP
	glm::ivec3( 0.0, -1.0,  0.0), //BOTTOM
};

// Vertices Data for each face
// A cube has 6 faces (sides)
// and each side has 4 vertices 
const glm::ivec3 CUBE_FACE_VERTICES_POS[6][4]
{
	//FRONT
	{
		glm::vec3(1.0, 1.0, 0.0), // Top Left
		glm::vec3(1.0, 0.0, 0.0), // Bot Left
		glm::vec3(0.0, 0.0, 0.0), // Bot Right
		glm::vec3(0.0, 1.0, 0.0), // Top Right
	},

	//BACK
	{
		glm::vec3(0.0, 1.0, 1.0), // Top Left
		glm::vec3(0.0, 0.0, 1.0), // Bot Left
		glm::vec3(1.0, 0.0, 1.0), // Bot Right
		glm::vec3(1.0, 1.0, 1.0), // Top Right
	},

	//LEFT
	{
		glm::vec3(0.0, 1.0, 0.0), //Top Left
		glm::vec3(0.0, 0.0, 0.0), //Bot Left
		glm::vec3(0.0, 0.0, 1.0), //Bot Right
		glm::vec3(0.0, 1.0, 1.0), //Top Right
	},

	//RIGHT
	{
		glm::vec3(1.0, 1.0, 1.0), //Top Left
		glm::vec3(1.0, 0.0, 1.0), //Bot Left
		glm::vec3(1.0, 0.0, 0.0), //Bot Right
		glm::vec3(1.0, 1.0, 0.0), //Top Right
	},


	//Top
	{
		glm::vec3(0.0, 1.0, 0.0), //Top Left
		glm::vec3(0.0, 1.0, 1.0), //Bot Left
		glm::vec3(1.0, 1.0, 1.0), //Bot Right
		glm::vec3(1.0, 1.0, 0.0), //Top Right
	},

	//Bottom
	{
		glm::vec3(0.0, 0.0, 1.0), //Top Left
		glm::vec3(0.0, 0.0, 0.0), //Bot Left
		glm::vec3(1.0, 0.0, 0.0), //Bot Right
		glm::vec3(1.0, 0.0, 1.0), //Top Right
	},
};


Chunk::Chunk()
{
	//Check if a saved chunk file exist
}

Chunk::~Chunk()
{
	//Save Data to disk
	std::string filePath = "game/data/world/" + std::to_string(m_ChunkPos.x) + "-" + std::to_string(m_ChunkPos.y) + ".bin";
	
	//Create and write file for this chunk
	std::ofstream file(filePath, std::ios::binary);
	//Write Data
	file.write(reinterpret_cast<const char*>(m_VoxelData), sizeof(m_VoxelData));
	
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
		saveFile.read(reinterpret_cast<char*>(m_VoxelData), sizeof(m_VoxelData));
	}
	else
	{
		//We Generate data
		PopulateVoxelData();
	}
	
}

void Chunk::SetVoxel(glm::ivec3 localPos, char voxelID)
{
	m_VoxelData[localPos.x][localPos.y][localPos.z] = voxelID;
	GenerateMesh();
}

char Chunk::CheckVoxel(glm::ivec3 localPos)
{
	return m_VoxelData[localPos.x][localPos.y][localPos.z];
}

void Chunk::PopulateVoxelData()
{
	for (int x = 0; x < CHUNK_LENGTH; x++)
	{
		for (int y = 0; y < CHUNK_HEIGHT; y++)
		{
			for (int z = 0; z < CHUNK_LENGTH; z++) 
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
	for (int x = 0; x < CHUNK_LENGTH; x++)
	{
		for (int y = 0; y < CHUNK_HEIGHT; y++)
		{
			for (int z = 0; z < CHUNK_LENGTH; z++)
			{
				//Position of this voxel
				glm::ivec3 voxelPos = glm::ivec3(x, y, z);

				//Get the voxel at position (x, y, z)
				char voxel = m_VoxelData[x][y][z];
				
				//Air doesnt neeed to be rendered
				if (voxel == BLOCK_ID::Air)
					continue;

				//Check Surrounding Blocks
				char temp;
				for (int face = 0; face < CUBE_FACES; face++)
				{
					glm::ivec3 normal = CUBE_FACE_NORMALS[face];
					
					//Check if there's a block infront of the face
					glm::ivec3 checkPos = voxelPos + normal;
					//Convert the position to world Space
					checkPos += glm::vec3(m_ChunkPos.x, 0.0, m_ChunkPos.y) * glm::vec3(CHUNK_LENGTH);

					char block = World::Get().GetVoxel(checkPos);
					
					//If the face has a block in front of it, then it is occluded so we dont need to render what we cant see
					if (block != BLOCK_ID::Air)
						continue;

					//Loop through all of the face's vertices
					for (int vert = 0; vert < NUM_CUBE_FACE_VERTS; vert++)
					{
						//get vertex position at this voxel pos
						glm::ivec3 pos = voxelPos + CUBE_FACE_VERTICES_POS[face][vert];

						//Add x, y, and z positions
						vertices.emplace_back(pos.x);
						vertices.emplace_back(pos.y);
						vertices.emplace_back(pos.z);

						//Add Normals
						vertices.emplace_back(normal.x);
						vertices.emplace_back(normal.y);
						vertices.emplace_back(normal.z);

						
						//Get the UV from the lookup table
						glm::vec2 uv = BLOCK_UVS[voxel][vert];

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
	glm::vec3 pos = glm::vec3(m_ChunkPos.x * CHUNK_LENGTH, 0.0f, m_ChunkPos.y * CHUNK_LENGTH);
	glm::mat4 modelMatrix = glm::mat4(1.0); //Initialize to Identity matrix
	modelMatrix = glm::translate(modelMatrix, pos);

	shader->SetMat4("a_ModelMatrix", modelMatrix);

	//Bind the Vertex Array that's about to be rendered
	m_ChunkVAO->Bind();
	
	//Render the VAO
	glDrawElements(GL_TRIANGLES, m_ChunkIBO->GetCount(), GL_UNSIGNED_INT, 0);
}