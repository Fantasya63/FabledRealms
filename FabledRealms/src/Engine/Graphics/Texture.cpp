#include "frpch.h"
#include "Texture.h"
#include "Debug/Debug.h"
#include "Shader.h"
#include "Mesh/Mesh.h"
#include "Engine/Application.h"

#include <stb_image.h>
#include <glad/glad.h>
#include <filesystem>
#include <glm/glm.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>


#define NUM_CUBEMAP_FACES 6


Texture::~Texture()
{
	glDeleteTextures(1, &m_RendererID);
}

void Texture::Bind() const
{
	switch (m_TextureType)
	{
	case Texture::TEXTURE_TYPE::TEXTURE2D:
		glBindTexture(GL_TEXTURE_2D, m_RendererID);
		break;
	case Texture::TEXTURE_TYPE::CUBEMAP:
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_RendererID);
		break;
	default:
		FR_CORE_ASSERT(false, "Unknown Texture Type");
		break;
	}
	
}


void Texture::SetTextureWrapAndFilter2D(TEXTURE_FILTER filter)
{
	//Set Wrap Options to Clamp To Edge
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

void Texture::InitTexture2D(const std::string& path, TEXTURE_FILTER filter, bool isColorData, bool generateMipmaps)
{
	FR_CORE_ASSERT(!m_RendererID, "Texture already initiallized!");

	stbi_set_flip_vertically_on_load(true);
	unsigned char* data;
	int width, height, numChannels;

	m_TextureType = TEXTURE_TYPE::TEXTURE2D;

	
	glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
	glBindTexture(GL_TEXTURE_2D, m_RendererID);

	//Load Texture Data From Disk
	data = stbi_load(path.c_str(), &width, &height, &numChannels, 0);
	FR_CORE_ASSERT(data, "Failed to load texture at : " << path);


	//We currently only support RGB and RGBA textures
	GLenum srcFormat, gpuFormat;
	if (numChannels == 4)
	{
		srcFormat = GL_RGBA;

		if (isColorData)
			gpuFormat = GL_SRGB8_ALPHA8;
		else
			gpuFormat = GL_RGBA;
	}
	else if (numChannels == 3)
	{
		srcFormat = GL_RGB;

		if (isColorData)
			gpuFormat = GL_SRGB8;
		else
			gpuFormat = GL_RGB;
	}
	else
	{
		FR_CORE_ASSERT(false, "Unknown texture channels");
		return;
	}


	// Upload Image Data to the GPU.
	// Takes in: Which target, mipmap level, format to store texture on gpu, width, height, legacy opengl (should always be 0), format of source, data type of src, image data, 
	glTexImage2D(GL_TEXTURE_2D, 0, gpuFormat, width, height, 0, srcFormat, GL_UNSIGNED_BYTE, data);


	//Tell the GPU to Generate Mipmaps
	if (generateMipmaps)
		glGenerateMipmap(GL_TEXTURE_2D);
	
		

	//Select Texture Filtering Options
	SetTextureWrapAndFilter2D(filter);

	stbi_image_free(data);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::InitCubemapTexture(const std::string path[6])
{
	FR_CORE_ASSERT(!m_RendererID, "Texture already initiallized!");


	unsigned char* data;
	int width, height, numChannels;

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
		data = stbi_load(path[i].c_str(), &width, &height, &numChannels, 0);
		FR_CORE_ASSERT(data, "Failed to load texture at : " << path[i]);
	
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

		SetTextureWrapAndFilterCubemap(TEXTURE_FILTER::LINEAR);

		stbi_image_free(data);
	}

	//Reenable Flipping on load
	stbi_set_flip_vertically_on_load(true);
	//glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}




void Texture::InitEquirectangularMap(const std::string& path, Texture* radianceTexture)
{
	//From https://learnopengl.com/PBR/IBL/Diffuse-irradiance
	m_TextureType = TEXTURE_TYPE::CUBEMAP;



	FR_CORE_ASSERT(!m_RendererID, "Texture already initiallized!");

	unsigned int hdrTexture;
	stbi_set_flip_vertically_on_load(true);
	int width, height, nrComponents;
	float* data = stbi_loadf(path.c_str(), &width, &height, &nrComponents, 0);
	FR_CORE_ASSERT(data, "Failed to load Equirectangular map at : " << path);

	glGenTextures(1, &hdrTexture);
	glBindTexture(GL_TEXTURE_2D, hdrTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, data);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);



	//Convert to Cubemap
	unsigned int captureFBO, captureRBO;
	glGenFramebuffers(1, &captureFBO);
	glGenRenderbuffers(1, &captureRBO);

	glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
	glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 512, 512);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, captureRBO);




	//Allocate memory for the 6 cubemap faces
	glGenTextures(1, &m_RendererID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_RendererID);
	for (unsigned int i = 0; i < NUM_CUBEMAP_FACES; ++i)
	{
		// note that we store each face with 16 bit floating point values
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F,
			512, 512, 0, GL_RGB, GL_FLOAT, nullptr);
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);




	//Render to cubemap
	Shader equirectangularToCubemapShader("Assets/Shaders/Core/EquirectangularToCubemapShader.vert",
		"Assets/Shaders/Core/EquirectangularToCubemapShader.frag");
	
	Mesh mesh;
	Mesh::InitMeshCubemap(mesh);
	
	glm::mat4 captureProjection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);
	glm::mat4 captureViews[] =
	{
	   glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
	   glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
	   glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f)),
	   glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f)),
	   glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
	   glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f))
	};

	// convert HDR equirectangular environment map to cubemap equivalent
	equirectangularToCubemapShader.Use();
	equirectangularToCubemapShader.SetInt("equirectangularMap", 0);
	equirectangularToCubemapShader.SetMat4("projection", captureProjection);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, hdrTexture);

	glViewport(0, 0, 512, 512); // don't forget to configure the viewport to the capture dimensions.
	glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
	glBindVertexArray(mesh.VAO);
	for (unsigned int i = 0; i < 6; ++i)
	{
		equirectangularToCubemapShader.SetMat4("view", captureViews[i]);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
			GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, m_RendererID, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL);
	}
	glBindVertexArray(0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);


	//Generate Radiance Map
	if (radianceTexture == nullptr)
	{
		//Cleanup
		glDeleteTextures(1, &hdrTexture);
		Mesh::CleanUpMesh(mesh);

		Window* window = Application::Get().GetWindow();
		glViewport(0, 0, window->GetWidth(), window->GetHeight());

		stbi_image_free(data);
		return;
	}
}
