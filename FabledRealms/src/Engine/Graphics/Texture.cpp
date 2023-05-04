#include "frpch.h"
#include "Texture.h"
#include "stb_image.h"
#include <glad/glad.h>
#include "Debug/Debug.h"

Texture::Texture(const char* texturePath)
{
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load(texturePath, &m_Width, &m_Height, &m_NumChannels, 0);

	if (!data)
	{
		LOG_CORE_ERROR("Failed to load texture at : " << texturePath);

#ifdef FR_DEBUG
		FR_CORE_ASSERT(false, "")
#endif // FR_DEBUG
	}

	//Activate Texture unit
	glActiveTexture(GL_TEXTURE0);

	//Create Textures
	glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
	glBindTexture(GL_TEXTURE_2D, m_RendererID);

	//Set Wrapping options
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	//Set Filter options - TODO- Use inputed Filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	// Which target, mipmap level, format to store texture on gpu, width, height, legacy opengl (should always be 0), format of source, data type of src, image data, 
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_Width, m_Height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);

	//free the cpu texture data
	stbi_image_free(data);
}

Texture::~Texture()
{
	glDeleteTextures(1, &m_RendererID);
}

void Texture::Bind() const
{
	glBindTexture(GL_TEXTURE_2D, m_RendererID);
}
