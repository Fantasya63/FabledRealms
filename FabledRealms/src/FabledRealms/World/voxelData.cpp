#include "frpch.h"
#include "VoxelData.h"
#include <glm/glm.hpp>
#include "Engine/Graphics/TextureAtlass.h"

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
			TextureAtlass::GetElementIndex("bedrock"), //FRONT
			TextureAtlass::GetElementIndex("bedrock"), //BACK
			TextureAtlass::GetElementIndex("bedrock"), //LEFT
			TextureAtlass::GetElementIndex("bedrock"), //RIGHT
			TextureAtlass::GetElementIndex("bedrock"), //TOP
			TextureAtlass::GetElementIndex("bedrock"), //BOTTOM
		}
	},
	//Stone
	{
		false,
		{
			TextureAtlass::GetElementIndex("stone"), //FRONT
			TextureAtlass::GetElementIndex("stone"), //BACK
			TextureAtlass::GetElementIndex("stone"), //LEFT
			TextureAtlass::GetElementIndex("stone"), //RIGHT
			TextureAtlass::GetElementIndex("stone"), //TOP
			TextureAtlass::GetElementIndex("stone"), //BOTTOM
		}
	},
	//Dirt
	{
		false,
		{
			TextureAtlass::GetElementIndex("dirt"), //FRONT
			TextureAtlass::GetElementIndex("dirt"), //BACK
			TextureAtlass::GetElementIndex("dirt"), //LEFT
			TextureAtlass::GetElementIndex("dirt"), //RIGHT
			TextureAtlass::GetElementIndex("dirt"), //TOP
			TextureAtlass::GetElementIndex("dirt"), //BOTTOM
		}
	},
	//Grass
	{
		false,
		{
			TextureAtlass::GetElementIndex("grass_block_side"), //FRONT
			TextureAtlass::GetElementIndex("grass_block_side"), //BACK
			TextureAtlass::GetElementIndex("grass_block_side"), //LEFT
			TextureAtlass::GetElementIndex("grass_block_side"), //RIGHT
			TextureAtlass::GetElementIndex("grass_block_top"),  //TOP
			TextureAtlass::GetElementIndex("dirt"),  //BOTTOM
		}
	},
	//Sand
	{
		false,
		{
			TextureAtlass::GetElementIndex("sand"), //FRONT
			TextureAtlass::GetElementIndex("sand"), //BACK
			TextureAtlass::GetElementIndex("sand"), //LEFT
			TextureAtlass::GetElementIndex("sand"), //RIGHT
			TextureAtlass::GetElementIndex("sand"), //TOP
			TextureAtlass::GetElementIndex("sand"), //BOTTOM
		}
	},
	//WoodPlack
	{
		false,
		{
			TextureAtlass::GetElementIndex("oak_planks"), //FRONT
			TextureAtlass::GetElementIndex("oak_planks"), //BACK
			TextureAtlass::GetElementIndex("oak_planks"), //LEFT 
			TextureAtlass::GetElementIndex("oak_planks"), //RIGHT
			TextureAtlass::GetElementIndex("oak_planks"), //TOP
			TextureAtlass::GetElementIndex("oak_planks"), //BOTTOM
		}
	},
	//WoodLog
	{
		false,
		{
			TextureAtlass::GetElementIndex("oak_log"), //FRONT
			TextureAtlass::GetElementIndex("oak_log"), //BACK
			TextureAtlass::GetElementIndex("oak_log"), //LEFT
			TextureAtlass::GetElementIndex("oak_log"), //RIGHT
			TextureAtlass::GetElementIndex("oak_log_top"), //TOP
			TextureAtlass::GetElementIndex("oak_log_top"), //BOTTOM
		}
	},
	//Brick,
	{
		false,
		{
			TextureAtlass::GetElementIndex("bricks"), //FRONT
			TextureAtlass::GetElementIndex("bricks"), //BACK
			TextureAtlass::GetElementIndex("bricks"), //LEFT
			TextureAtlass::GetElementIndex("bricks"), //RIGHT
			TextureAtlass::GetElementIndex("bricks"), //TOP
			TextureAtlass::GetElementIndex("bricks"), //BOTTOM
		}
	},
	//Leaves
	{
		true,
		{
			TextureAtlass::GetElementIndex("oak_leaves"), //FRONT
			TextureAtlass::GetElementIndex("oak_leaves"), //BACK
			TextureAtlass::GetElementIndex("oak_leaves"), //LEFT
			TextureAtlass::GetElementIndex("oak_leaves"), //RIGHT
			TextureAtlass::GetElementIndex("oak_leaves"), //TOP
			TextureAtlass::GetElementIndex("oak_leaves"), //BOTTOM
		}
	},
	//BlackWool
	{
		false,
		{
			TextureAtlass::GetElementIndex("black_wool"), //FRONT
			TextureAtlass::GetElementIndex("black_wool"), //BACK
			TextureAtlass::GetElementIndex("black_wool"), //LEFT
			TextureAtlass::GetElementIndex("black_wool"), //RIGHT
			TextureAtlass::GetElementIndex("black_wool"), //TOP
			TextureAtlass::GetElementIndex("black_wool"), //BOTTOM
		}
	},
	//Orange Wool
	{
		false,
		{
			TextureAtlass::GetElementIndex("orange_wool"), //FRONT
			TextureAtlass::GetElementIndex("orange_wool"), //BACK
			TextureAtlass::GetElementIndex("orange_wool"), //LEFT
			TextureAtlass::GetElementIndex("orange_wool"), //RIGHT
			TextureAtlass::GetElementIndex("orange_wool"), //TOP
			TextureAtlass::GetElementIndex("orange_wool"), //BOTTOM
		}
	},
	//Red Wool
	{
		false,
		{
			TextureAtlass::GetElementIndex("red_wool"), //FRONT
			TextureAtlass::GetElementIndex("red_wool"), //BACK
			TextureAtlass::GetElementIndex("red_wool"), //LEFT
			TextureAtlass::GetElementIndex("red_wool"), //RIGHT
			TextureAtlass::GetElementIndex("red_wool"), //TOP
			TextureAtlass::GetElementIndex("red_wool"), //BOTTOM
		}
	},
	//WhiteWool,
	{
		false,
		{
			TextureAtlass::GetElementIndex("white_wool"), //FRONT
			TextureAtlass::GetElementIndex("white_wool"), //BACK
			TextureAtlass::GetElementIndex("white_wool"), //LEFT
			TextureAtlass::GetElementIndex("white_wool"), //RIGHT
			TextureAtlass::GetElementIndex("white_wool"), //TOP
			TextureAtlass::GetElementIndex("white_wool"), //BOTTOM
		}
	},
	//YellowWool
	{
		false,
		{
			TextureAtlass::GetElementIndex("yellow_wool"), //FRONT
			TextureAtlass::GetElementIndex("yellow_wool"), //BACK
			TextureAtlass::GetElementIndex("yellow_wool"), //LEFT
			TextureAtlass::GetElementIndex("yellow_wool"), //RIGHT
			TextureAtlass::GetElementIndex("yellow_wool"), //TOP
			TextureAtlass::GetElementIndex("yellow_wool"), //BOTTOM
		}
	},

	//gold
	{
		false,
		{
			TextureAtlass::GetElementIndex("gold_block"), //FRONT
			TextureAtlass::GetElementIndex("gold_block"), //BACK
			TextureAtlass::GetElementIndex("gold_block"), //LEFT
			TextureAtlass::GetElementIndex("gold_block"), //RIGHT
			TextureAtlass::GetElementIndex("gold_block"), //TOP
			TextureAtlass::GetElementIndex("gold_block"), //BOTTOM
		}
	},

	//gold
	{
		false,
		{
			TextureAtlass::GetElementIndex("gold_ore"), //FRONT
			TextureAtlass::GetElementIndex("gold_ore"), //BACK
			TextureAtlass::GetElementIndex("gold_ore"), //LEFT
			TextureAtlass::GetElementIndex("gold_ore"), //RIGHT
			TextureAtlass::GetElementIndex("gold_ore"), //TOP
			TextureAtlass::GetElementIndex("gold_ore"), //BOTTOM
		}
	},

	//white concrete
	{
		false,
		{
			TextureAtlass::GetElementIndex("white_concrete"), //FRONT
			TextureAtlass::GetElementIndex("white_concrete"), //BACK
			TextureAtlass::GetElementIndex("white_concrete"), //LEFT
			TextureAtlass::GetElementIndex("white_concrete"), //RIGHT
			TextureAtlass::GetElementIndex("white_concrete"), //TOP
			TextureAtlass::GetElementIndex("white_concrete"), //BOTTOM
		}
	},

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
		glm::vec3(1.0, 1.0, 0.0), // Top Right
		glm::vec3(1.0, 0.0, 0.0), // Bot Right
		glm::vec3(0.0, 0.0, 0.0), // Bot Left
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
		{ glm::ivec3(1, 0, -1), glm::ivec3(0,  1, -1), glm::ivec3(1,  1, -1)}, //TL
		{ glm::ivec3(1, 0, -1), glm::ivec3(0, -1, -1), glm::ivec3(1, -1, -1)}, //BL
		{ glm::ivec3(-1, 0, -1), glm::ivec3(0, -1, -1), glm::ivec3(-1, -1, -1)}, //BR
		{ glm::ivec3(-1, 0, -1), glm::ivec3(0,  1, -1), glm::ivec3(-1,  1, -1)}, //TR 
	},

	//Back
	{
		{ glm::ivec3(-1,  0, 1), glm::ivec3(0,  1, 1), glm::ivec3(-1,  1, 1)}, //TL 
		{ glm::ivec3(-1,  0, 1), glm::ivec3(0, -1, 1), glm::ivec3(-1, -1, 1)}, //BL
		{ glm::ivec3(1,  0, 1), glm::ivec3(0, -1, 1), glm::ivec3(1,  -1, 1)}, //BR
		{ glm::ivec3(1,  0, 1), glm::ivec3(0,  1, 1), glm::ivec3(1,  1,  1)}, //TR 
	},

	//Left
	{
		{ glm::ivec3(-1,  0, -1), glm::ivec3(-1,  1, 0), glm::ivec3(-1,  1, -1)}, //TL
		{ glm::ivec3(-1,  0, -1), glm::ivec3(-1, -1, 0), glm::ivec3(-1, -1, -1)}, //BL
		{ glm::ivec3(-1,  0,  1), glm::ivec3(-1, -1, 0), glm::ivec3(-1, -1,  1)}, //BR
		{ glm::ivec3(-1,  0,  1), glm::ivec3(-1,  1, 0), glm::ivec3(-1,  1,  1)}, //TR 
	},

	//Right
	{
		{ glm::ivec3(1,  0,  1), glm::ivec3(1,  1, 0), glm::ivec3(1,  1,  1)}, //TL
		{ glm::ivec3(1,  0,  1), glm::ivec3(1, -1, 0), glm::ivec3(1, -1,  1)}, //BL
		{ glm::ivec3(1,  0, -1), glm::ivec3(1, -1, 0), glm::ivec3(1,  -1, -1)}, //BR
		{ glm::ivec3(1,  0, -1), glm::ivec3(1,  1, 0), glm::ivec3(1,  1,  -1)}, //TR 
	},

	//Top
	{
		{ glm::ivec3(-1,  1, 0), glm::ivec3(0,  1, -1), glm::ivec3(-1,  1, -1)}, //TL 
		{ glm::ivec3(-1,  1, 0), glm::ivec3(0,  1,  1), glm::ivec3(-1,  1,  1)}, //BL
		{ glm::ivec3(1,  1,  0), glm::ivec3(0,  1,  1), glm::ivec3(1,  1,   1)}, //BR
		{ glm::ivec3(1,  1,  0), glm::ivec3(0,  1, -1), glm::ivec3(1,  1,  -1)}, //TR
	},

	//Bottom
	{
		{ glm::ivec3(-1, -1, 0), glm::ivec3(0,  -1,  1), glm::ivec3(-1, -1,  1)}, //TL
		{ glm::ivec3(-1, -1, 0), glm::ivec3(0,  -1, -1), glm::ivec3(-1, -1, -1)}, //BL
		{ glm::ivec3(1,  -1,  0), glm::ivec3(1, -1, -1), glm::ivec3(1,  -1, -1)}, //BR
		{ glm::ivec3(1,  -1,  0), glm::ivec3(1, -1,  1), glm::ivec3(1,  -1,  1)}, //TR 
	},

};

glm::vec2 VoxelData::GetUVSFromUVIndex(int index, int vert)
{

	//Get x,y positions
	glm::vec2 texturePos;
	texturePos.x = index % TextureAtlass::TextureAtlasSizeX;
	texturePos.y = index / TextureAtlass::TextureAtlasSizeX;
	
	//Convert texturePos to 0-1
	glm::vec2 textureAtlasSize = glm::vec2(TextureAtlass::TextureAtlasSizeX, TextureAtlass::TextureAtlasSizeY);
	texturePos /= textureAtlasSize;

	//texturePos.y = 1.0 - texturePos.y;


	//Add offset depending on what vert it is
	glm::vec2 uv = VoxelData::RAW_UV[vert] / textureAtlasSize;
	texturePos += uv;

	//Invert y as the index starts form the top left
	texturePos.y = 1.0 - texturePos.y;

	return texturePos;
}
