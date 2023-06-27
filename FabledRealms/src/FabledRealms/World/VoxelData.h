#pragma once
#include <glm/glm.hpp>

// Properties for each voxel/block type
struct VoxelProps
{
	bool IsTransparent;
	int	UVIndex[6]; // UV Index per face

	// For future endevours
	// char hardness; // determines how fast we can break the block
	// char stackAmount; // Total number of  items we can carry in a stack
};

struct AOSurroundingData
{
	glm::ivec3 side1;
	glm::ivec3 side2;
	glm::ivec3 corner;
};

//Voxel datas and Helper functions
class VoxelData
{
public:
	// ------------------------------------------ Voxel ------------------------------------------

	// Block Ids of each block type
	// Corresponds to the index of the Voxel Properties array
	enum BLOCK_ID
	{
		Air,		// 0
		Bedrock,	// 1
		Stone,		// 2
		Dirt,		// 3
		Grass,		// 4
		Sand,		// 5
		WoodPlank,	// 6
		WoodLog,	// 7
		Brick,		// 8
		Leaves,		// 9
		BlackWool,	// 10
		OrangeWool,	// 11
		RedWool,	// 12
		WhiteWool,	// 13
		YellowWool,	// 14
	};
	static const VoxelProps voxelProps[];
	static const int TOTAL_NUM_OF_BLOCKS; //Total number of blockTypes we have in the game

	static const int TOTAL_NUM_OF_CUBE_FACES = 6;
	static const int TOTAL_NUMBER_OF_CUBE_VERTS = 4;


	static const glm::vec3 CUBE_FACE_NORMALS[TOTAL_NUM_OF_CUBE_FACES];
	static const AOSurroundingData AO_SURROUNDING_DATA[TOTAL_NUM_OF_CUBE_FACES][4];

	// Vertices Data for each face
	// A cube has 6 faces (sides)
	// and each side has 4 vertices 
	static const glm::ivec3 CUBE_FACE_VERTICES_POS[6][4];

	static glm::vec2 GetUVSFromUVIndex(int index, int vert);

private:
	//The number of textures packed in the texture atlass in the x and y directions
	static const int TEXTURE_ATLAS_COUNT_X = 4;
	static const int TEXTURE_ATLAS_COUNT_Y = 4;

	//Raw 0-1 data for UVS in each face
	static const glm::vec2 RAW_UV[TOTAL_NUMBER_OF_CUBE_VERTS];

};