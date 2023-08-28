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
	Mesh::CleanUpMesh(m_ChunkMesh);

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

float Chunk::CalculateAO(const AOSurroundingData& aoData, glm::ivec3& pos)
{
	char side1ID = World::Get().GetVoxel(pos + aoData.side1);
	char side2ID = World::Get().GetVoxel(pos + aoData.side2);
	char cornerID = World::Get().GetVoxel(pos + aoData.corner);

	int side1 = !VoxelData::voxelProps[side1ID].IsTransparent;
	int side2 = !VoxelData::voxelProps[side2ID].IsTransparent;
	int corner = !VoxelData::voxelProps[cornerID].IsTransparent;

	if (side1 && side2) {
		return 0.0f;
	}
	return (3.0f - (side1 + side2 + corner)) / 3.0f;
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

	for (int x = 0; x < Chunk::CHUNK_LENGTH; x++)
	{
		for (int z = 0; z < Chunk::CHUNK_LENGTH; z++)
		{
			//Get Heightmap data from procedural noise
			//Get worldspace coordinates for this position
			glm::ivec2 world = (m_ChunkPos * glm::ivec2(World::WORLD_LENGTH)) + glm::ivec2(x, z);

			//Get Heightmap data
			Heightmap[x][z] = noise.GetNoise(static_cast<float>(world.x), static_cast<float>(world.y));

			for (int y = 0; y < Chunk::CHUNK_HEIGHT; y++)
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
	if (m_ChunkMesh.m_IsInitialized)
	{
		Mesh::CleanUpMesh(m_ChunkMesh);
	}


	std::vector<Vertex> vertices;
	vertices.reserve(1024);

	std::vector<uint32_t> indices;
	indices.reserve(1024);
		

	//For keeping track of the index values
	uint32_t currentIndex = 0;

	//Loop throught every voxel in the chunk
	for (int x = 0; x < Chunk::CHUNK_LENGTH; x++)
	{
		for (int y = 0; y < Chunk::CHUNK_HEIGHT; y++)
		{
			for (int z = 0; z < Chunk::CHUNK_LENGTH; z++)
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
					checkPos += glm::vec3(m_ChunkPos.x, 0.0, m_ChunkPos.y) * glm::vec3(Chunk::CHUNK_LENGTH);

					char blockCheckID = World::Get().GetVoxel(checkPos);
					
					//If the face has a non-transparent block in front of it, then it is occluded so we dont need to render what we cant see
					if (!VoxelData::voxelProps[blockCheckID].IsTransparent)
						continue;

					int uvIndex = VoxelData::voxelProps[voxelID].UVIndex[face];


					std::vector<Vertex> faceVerts;
					faceVerts.reserve(4);



					//Loop through all of the face's vertices
					for (int vert = 0; vert < VoxelData::TOTAL_NUMBER_OF_CUBE_VERTS; vert++)
					{
						Vertex vertex;

						//get vertex position at this voxel pos
						vertex.Position = voxelPos + VoxelData::CUBE_FACE_VERTICES_POS[face][vert];


						//Get the UV from the lookup table
						vertex.UV1 = VoxelData::GetUVSFromUVIndex(uvIndex, vert);


						//Add Normals
						vertex.Normal = normal;
						
						//Calculate AO
						const AOSurroundingData& aoData = VoxelData::AO_SURROUNDING_DATA[face][vert];


						vertex.AO = CalculateAO(aoData, voxelPos + glm::ivec3(m_ChunkPos.x, 0.0, m_ChunkPos.y) * glm::ivec3(Chunk::CHUNK_LENGTH));

						faceVerts.emplace_back(vertex);
					}

					//Calculate Tangent
					glm::vec3 tangent = Mesh::GetTangentVector(faceVerts);
					faceVerts[0].Tangent = tangent;
					faceVerts[1].Tangent = tangent;
					faceVerts[2].Tangent = tangent;
					faceVerts[3].Tangent = tangent;

					//Append to vertices
					//vertices.insert(vertices.end(), faceVerts.begin(), faceVerts.end());
					vertices.emplace_back(faceVerts[0]);
					vertices.emplace_back(faceVerts[1]);
					vertices.emplace_back(faceVerts[2]);
					vertices.emplace_back(faceVerts[3]);

					// Add Indices
					// These are the indices of the vertex that make up a Face
					// Each face has two triangles to make a square
					if (faceVerts[1].AO + faceVerts[3].AO > faceVerts[0].AO + faceVerts[2].AO) {
						// generate flipped quad

						// First Triangle
						indices.emplace_back(currentIndex + 3);
						indices.emplace_back(currentIndex);
						indices.emplace_back(currentIndex + 1);

						// Second Triangle
						indices.emplace_back(currentIndex + 3);
						indices.emplace_back(currentIndex + 1);
						indices.emplace_back(currentIndex + 2);
					}
					else {
						// generate normal quad

						// First Triangle
						indices.emplace_back(currentIndex);
						indices.emplace_back(currentIndex + 1);
						indices.emplace_back(currentIndex + 2);

						// Second Triangle
						indices.emplace_back(currentIndex);
						indices.emplace_back(currentIndex + 2);
						indices.emplace_back(currentIndex + 3);
					}
					

					// Increment current index by 4.
					// as there are 4 vertices per face
					currentIndex += 4;
				}
			}
		}
	}

	Mesh::InitMeshChunk(m_ChunkMesh, vertices, indices);
}

void Chunk::RenderChunk(Shader* shader, const glm::mat4& view)
{
	shader->Use();

	// Set Model Matrix
	// This is the position, rotation, and scale of the Chunk
	// In this case, we're only moving the chunk bby a certain amount
	glm::vec3 pos = glm::vec3(m_ChunkPos.x * Chunk::CHUNK_LENGTH, 0.0f, m_ChunkPos.y * Chunk::CHUNK_LENGTH);
	glm::mat4 modelMatrix = glm::mat4(1.0); //Initialize to Identity matrix
	modelMatrix = glm::translate(modelMatrix, pos);

	shader->SetMat4("a_ModelMatrix", modelMatrix);
	shader->SetMat4("a_ModelViewMatrix", view * modelMatrix);

	glm::mat3 viewNormalMatrix = glm::transpose(glm::inverse(view * modelMatrix));


	shader->SetMat3("a_ViewNormalMatrix", viewNormalMatrix);
	
		//vs_out.Normal = transpose(inverse(mat3(model))) * aNormal;
	
	//Render Mesh
	m_ChunkMesh.RenderMesh(*shader);
	glUseProgram(0);
}