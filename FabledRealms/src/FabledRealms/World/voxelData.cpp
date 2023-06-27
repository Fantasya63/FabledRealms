#include "frpch.h"
#include "VoxelData.h"
#include <glm/glm.hpp>

//Voxel Properties
const VoxelProps VoxelData::voxelProps[] = {

	//Transparency, UVIndex Array
	
	//Air
	{
		true,
		{}
	},
	//Bedrock
	{
		false,
		{
			9, //FRONT
			9, //BACK
			9, //LEFT
			9, //RIGHT
			9, //TOP
			9, //BOTTOM
		}
	},
	//Stone
	{
		false,
		{
			11, //FRONT
			11, //BACK
			11, //LEFT
			11, //RIGHT
			11, //TOP
			11, //BOTTOM
		}
	},
	//Dirt
	{
		false,
		{
			14, //FRONT
			14, //BACK
			14, //LEFT
			14, //RIGHT
			14, //TOP
			14, //BOTTOM
		}
	},
	//Grass
	{
		false,
		{
			8, //FRONT
			8, //BACK
			8, //LEFT
			8, //RIGHT
			15, //TOP
			14, //BOTTOM
		}
	},
	//Sand
	{
		false,
		{
			10, //FRONT
			10, //BACK
			10, //LEFT
			10, //RIGHT
			10, //TOP
			10, //BOTTOM
		}
	},
	//WoodPlack
	{
		false,
		{
			6, //FRONT
			6, //BACK
			6, //LEFT 
			6, //RIGHT
			6, //TOP
			6, //BOTTOM
		}
	},
	//WoodLog
	{
		false,
		{
			4, //FRONT
			4, //BACK
			4, //LEFT
			4, //RIGHT
			5, //TOP
			5, //BOTTOM
		}
	},
	//Brick,
	{
		false,
		{
			13, //FRONT
			13, //BACK
			13, //LEFT
			13, //RIGHT
			13, //TOP
			13, //BOTTOM
		}
	},
	//Leaves
	{
		false,
		{
			12, //FRONT
			12, //BACK
			12, //LEFT
			12, //RIGHT
			12, //TOP
			12, //BOTTOM
		}
	},
	//BlackWool
	{
		false,
		{
			7, //FRONT
			7, //BACK
			7, //LEFT
			7, //RIGHT
			7, //TOP
			7, //BOTTOM
		}
	},
	//Orange Wool
	{
		false,
		{
			0, //FRONT
			0, //BACK
			0, //LEFT
			0, //RIGHT
			0, //TOP
			0, //BOTTOM
		}
	},
	//Red Wool
	{
		false,
		{
			1, //FRONT
			1, //BACK
			1, //LEFT
			1, //RIGHT
			1, //TOP
			1, //BOTTOM
		}
	},
	//WhiteWool,
	{
		false,
		{
			2, //FRONT
			2, //BACK
			2, //LEFT
			2, //RIGHT
			2, //TOP
			2, //BOTTOM
		}
	},
	//YellowWool
	{
		false,
		{
			3, //FRONT
			3, //BACK
			3, //LEFT
			3, //RIGHT
			3, //TOP
			3, //BOTTOM
		}
	}
};

//Calculate the number of blocktypes we have
const int VoxelData::TOTAL_NUM_OF_BLOCKS = sizeof(VoxelData::voxelProps) / sizeof(VoxelData::voxelProps[0]);

//Normal data for each face
const glm::vec3 VoxelData::CUBE_FACE_NORMALS [VoxelData::TOTAL_NUM_OF_CUBE_FACES] = {
	glm::ivec3(0,  0, -1), //FRONT
	glm::ivec3(0,  0,  1), //BACK
	glm::ivec3(-1,  0,  0),//LEFT
	glm::ivec3(1,  0,  0), //RIGHT
	glm::ivec3(0,  1,  0), //TOP
	glm::ivec3(0, -1,  0), //BOTTOM
};

// Vertices Data for each face
// A cube has 6 faces (sides)
// and each side has 4 vertices 
const glm::ivec3 VoxelData::CUBE_FACE_VERTICES_POS [VoxelData::TOTAL_NUM_OF_CUBE_FACES][VoxelData::TOTAL_NUMBER_OF_CUBE_VERTS] =
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


const glm::vec2 VoxelData::RAW_UV[VoxelData::TOTAL_NUMBER_OF_CUBE_VERTS] = 
{
	glm::vec2(0.0, 1.0), // Top Left
	glm::vec2(0.0, 0.0), // Bottom Left
	glm::vec2(1.0, 0.0), // Bottom Right
	glm::vec2(1.0, 1.0), // Top Right
};

const AOSurroundingData VoxelData::AO_SURROUNDING_DATA[VoxelData::TOTAL_NUM_OF_CUBE_FACES][4] =
{
		//Side 1				Side 2					Corner	

	//Front
	{
		{ glm::ivec3(-1, 0, -1), glm::ivec3(0,  1, -1), glm::ivec3(-1,  1, -1)}, //TL 
		{ glm::ivec3(-1, 0, -1), glm::ivec3(0, -1, -1), glm::ivec3(1,  1, -1)}, //BL
		{ glm::ivec3( 1, 0, -1), glm::ivec3(1, -1, -1), glm::ivec3(1, -1, -1)}, //BR
		{ glm::ivec3( 1, 0, -1), glm::ivec3(1,  1, -1), glm::ivec3(1,  1, -1)}, //TR
	},

	//Back
	{
		{ glm::ivec3( 1,  0, 1), glm::ivec3(0,  1, 1), glm::ivec3(1,  1,  1)}, //TL 
		{ glm::ivec3( 1,  0, 1), glm::ivec3(0, -1, 1), glm::ivec3(1,  -1, 1)}, //BL
		{ glm::ivec3(-1,  0, 1), glm::ivec3(0, -1, 1), glm::ivec3(-1, -1, 1)}, //BR
		{ glm::ivec3(-1,  0, 1), glm::ivec3(0,  1, 1), glm::ivec3(-1,  1, 1)}, //TR 
	},

	//Left
	{
		{ glm::ivec3(-1,  0,  1), glm::ivec3(-1,  1, 0), glm::ivec3(-1,  1,  1)}, //TL 
		{ glm::ivec3(-1,  0,  1), glm::ivec3(-1, -1, 0), glm::ivec3(-1, -1,  1)}, //BL
		{ glm::ivec3(-1,  0, -1), glm::ivec3(-1, -1, 0), glm::ivec3(-1, -1, -1)}, //BR
		{ glm::ivec3(-1,  0, -1), glm::ivec3(-1,  1, 0), glm::ivec3(-1,  1, -1)}, //TR 
	},

	//Right
	{
		{ glm::ivec3(1,  0, -1), glm::ivec3(1,  1, 0), glm::ivec3(1,  1,  -1)}, //TL 
		{ glm::ivec3(1,  0, -1), glm::ivec3(1, -1, 0), glm::ivec3(1,  -1, -1)}, //BL
		{ glm::ivec3(1,  0,  1), glm::ivec3(1, -1, 0), glm::ivec3(1, -1,  1)}, //BR
		{ glm::ivec3(1,  0,  1), glm::ivec3(1,  1, 0), glm::ivec3(1,  1,  1)}, //TR 
	},

	//Top
	{
		{ glm::ivec3(-1,  1, 0), glm::ivec3(0,  1,  1), glm::ivec3(-1,  1,  1)}, //TL 
		{ glm::ivec3(-1,  1, 0), glm::ivec3(0,  1, -1), glm::ivec3(-1,  1, -1)}, //BL
		{ glm::ivec3(1,  1,  0), glm::ivec3(0,  1, -1), glm::ivec3(1,  1,  -1)}, //BR
		{ glm::ivec3(1,  1,  0), glm::ivec3(0,  1,  1), glm::ivec3(1,  1,  1)}, //TR 
	},

	//Bottom
	{
		{ glm::ivec3(-1, -1, 0), glm::ivec3(0,  -1, -1), glm::ivec3(-1, -1, -1)}, //TL 
		{ glm::ivec3(-1, -1, 0), glm::ivec3(0,  -1,  1), glm::ivec3(-1, -1,  1)}, //BL
		{ glm::ivec3(1,  -1,  0), glm::ivec3(1, -1,  1), glm::ivec3(1,  -1,  1)}, //BR
		{ glm::ivec3(1,  -1,  0), glm::ivec3(1, -1, -1), glm::ivec3(1,  -1, -1)}, //TR 
	},

};

glm::vec2 VoxelData::GetUVSFromUVIndex(int index, int vert)
{

	//Get x,y positions
	glm::vec2 texturePos;
	texturePos.x = index % TEXTURE_ATLAS_COUNT_X;
	texturePos.y = glm::floor(index / TEXTURE_ATLAS_COUNT_X);

	//Convert texturePos to 0-1
	glm::vec2 textureAtlasSize = glm::vec2(TEXTURE_ATLAS_COUNT_X, TEXTURE_ATLAS_COUNT_Y);
	texturePos /= textureAtlasSize;

	glm::vec2 test = VoxelData::RAW_UV[vert] / textureAtlasSize;
	//Add offset depending on what vert it is
	texturePos += test;

	//Test
	return VoxelData::RAW_UV[vert];
	
	return texturePos;
}
