#include "frpch.h"
#include "Texture.h"
#include "stb_image.h"
#include <glad/glad.h>
#include "Debug/Debug.h"

#define NUM_CUBEMAP_FACES 6

Texture::Texture(const char texturePaths[6][100], TEXTURE_TYPE type, TEXTURE_FILTER filter)
{
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data;
	int width, height, numChannels;


	// ----------------------------- TEXTURE2D ------------------------------------

	if (type == TEXTURE_TYPE::TEXTURE2D)
	{
		m_TextureType = TEXTURE_TYPE::TEXTURE2D;

		//Create OpenGLTexture of the appropriate type and Bind it
		glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
		glBindTexture(GL_TEXTURE_2D, m_RendererID);

		//Load Texture Data From Disk
		data = stbi_load(texturePaths[0], &width, &height, &numChannels, 0);
		if (!data)
		{
			LOG_CORE_ERROR("Failed to load texture at : " << texturePaths[0]);
			FR_CORE_ASSERT(false, ""); //Insert A breakpoint
		}

		//We currently only support RGB and RGBA textures
		int srcFormat, gpuFormat;
		if (numChannels == 4)
		{
			srcFormat = GL_RGBA;
			gpuFormat = GL_SRGB8_ALPHA8;
		}
		else
		{
			srcFormat = GL_RGB;
			gpuFormat = GL_SRGB;
		}
		

		// Upload Image Data to the GPU.
		// Takes in: Which target, mipmap level, format to store texture on gpu, width, height, legacy opengl (should always be 0), format of source, data type of src, image data, 
		glTexImage2D(GL_TEXTURE_2D, 0, gpuFormat, width, height, 0, srcFormat, GL_UNSIGNED_BYTE, data);


		//Tell the GPU to Generate Mipmaps
		glGenerateMipmap(GL_TEXTURE_2D);

		//Select Texture Filtering Options
		SetTextureWrapAndFilter2D(filter);

		stbi_image_free(data);
	}

	// ------------------------------ CUBEMAP ------------------------------------

	else if (type == TEXTURE_TYPE::CUBEMAP)
	{
		//Apparently, On Cubemaps, textures are flipped upside down when using this, so we disable it for the moment
		stbi_set_flip_vertically_on_load(false);

		m_TextureType = TEXTURE_TYPE::CUBEMAP;

		//Create OpenGLTexture of the appropriate type and Bind it
		glCreateTextures(GL_TEXTURE_CUBE_MAP, 1, &m_RendererID);
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_RendererID);

		//Load Textures' Data From Disk
		//Load textures one by one then upload it to the GPU
		for (int i = 0; i < NUM_CUBEMAP_FACES; i++)
		{
			data = stbi_load(texturePaths[i], &width, &height, &numChannels, 0);
			
			if (!data)
			{
				LOG_CORE_ERROR("Failed to load texture at : " << texturePaths[i]);
				FR_CORE_ASSERT(false, ""); //Insert A breakpoint
			}

			//We currently only support RGB and RGBA textures
			int srcFormat, gpuFormat;
			if (numChannels == 4)
			{
				srcFormat = GL_RGBA;
				gpuFormat = GL_SRGB8_ALPHA8;
			}
			else
			{
				srcFormat = GL_RGB;
				gpuFormat = GL_SRGB;
			}

			//In OpenGl, the enums: 
			// GL_TEXTURE_CUBE_MAP_POSITIVE_X (Right)
			// GL_TEXTURE_CUBE_MAP_NEGATIVE_X (Left)
			// GL_TEXTURE_CUBE_MAP_POSITIVE_Y (Top)
			// GL_TEXTURE_CUBE_MAP_NEGATIVE_Y (Bottom)
			// GL_TEXTURE_CUBE_MAP_POSITIVE_Z (Front)
			// GL_TEXTURE_CUBE_MAP_NEGATIVE_Z (Back)
			// 
			// are next to each other by 1. So we can loop through the respective textureTargets by adding an i amount to
			// GL_TEXTURE_CUBE_MAP_POSITIVE_X
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, gpuFormat, width, height, 0, srcFormat, GL_UNSIGNED_BYTE, data);
		
			SetTextureWrapAndFilterCubemap(filter);

			stbi_image_free(data);
		}

		//Reenable Flipping on load
		stbi_set_flip_vertically_on_load(true);
	}

	// ------------------------------ UNKNOWN TYPE -------------------------------S

	else
	{
		FR_CORE_ASSERT(false, "ERROR! Unknown Texture type!");
	}
}


Texture::~Texture()
{
	glDeleteTextures(1, &m_RendererID);
}

void Texture::Bind() const
{
	glBindTexture(GL_TEXTURE_2D, m_RendererID);
}


void Texture::SetTextureWrapAndFilter2D(TEXTURE_FILTER filter)
{
	//Set Wrap Options to REPEAT sothat the texture repeats when the UV is outside 0 to 1 range
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	//Set Filter Options
	switch (filter)
	{
		case Texture::TEXTURE_FILTER::NEAREST:
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			break;

		case Texture::TEXTURE_FILTER::LINEAR:
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			break;

		default:
			LOG_CORE_ERROR("Unknown Texture Filter Option");
			FR_CORE_ASSERT(false, "");
			break;
	}
}


void Texture::SetTextureWrapAndFilterCubemap(TEXTURE_FILTER filter)
{
	// Set Wrap Options of the Cubemap to Clamp To Edge to avoid seams at the edge of the cubemap
	// Due to floatiing point precision errors
	// Note, 
	// S is equivalent to the UV's x, 
	// T is equivalent to the UV's y,
	// R is equivalent to the UV's z, 
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	//Set Filter Options
	switch (filter)
	{
		case TEXTURE_FILTER::NEAREST:
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			break;

		case TEXTURE_FILTER::LINEAR:
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			break;

		default:
			LOG_CORE_ERROR("Unknown Texture Filter Option");
			FR_CORE_ASSERT(false, "");
			break;
	}
}
