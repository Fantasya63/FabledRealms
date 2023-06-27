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

void Texture::Bind(uint32_t slot) const
{
	switch (m_TextureType)
	{
	case Texture::TEXTURE_TYPE::TEXTURE2D:
		glBindTextureUnit(slot, m_RendererID);
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
	glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
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




void GetTextureSrcAndGPUFormat(GLenum& src, GLenum& gpu, uint32_t numChannels, bool isColorData)
{
	//We currently only support RGB and RGBA textures
	if (numChannels == 4)
	{
		src = GL_RGBA;

		if (isColorData)
			gpu = GL_SRGB8_ALPHA8;
		else
			gpu = GL_RGBA8;
	}
	else if (numChannels == 3)
	{
		src = GL_RGB;

		if (isColorData)
			gpu = GL_SRGB8;
		else
			gpu = GL_RGB8;
	}
	else
	{
		FR_CORE_ASSERT(false, "Unknown texture channels");
		return;
	}
}


void Texture::InitTexture2D(const std::string& path, TEXTURE_FILTER filter, bool isColorData, bool generateMipmaps)
{
	FR_CORE_ASSERT(!m_RendererID, "Texture already initiallized!");

	stbi_set_flip_vertically_on_load(true);
	unsigned char* data;
	int width, height, numChannels;

	m_TextureType = TEXTURE_TYPE::TEXTURE2D;

	
	//glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
	glGenTextures(1, &m_RendererID);
	glBindTexture(GL_TEXTURE_2D, m_RendererID);

	//Load Texture Data From Disk
	data = stbi_load(path.c_str(), &width, &height, &numChannels, 0);
	FR_CORE_ASSERT(data, "Failed to load texture at : " << path);

	

	m_Width = width;
	m_Height = height;



	//We currently only support RGB and RGBA textures
	GLenum srcFormat, gpuFormat;
	GetTextureSrcAndGPUFormat(srcFormat, gpuFormat, numChannels, isColorData);

	//uint32_t levels = static_cast<uint32_t>(std::floor(std::log2(std::max(m_Width, m_Height)))) + 1;
	//glTextureStorage2D(m_RendererID, 1, gpuFormat, m_Width, m_Height);

	//Set Filter Options
	switch (filter)
	{
	case Texture::TEXTURE_FILTER::NEAREST:
		//glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		//glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		break;

	case Texture::TEXTURE_FILTER::LINEAR:
		//glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		//glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		break;

	default:
		LOG_CORE_ERROR("Unknown Texture Filter Option");
		FR_CORE_ASSERT(false, "");
		break;
	}

	glTexImage2D(GL_TEXTURE_2D, 0, gpuFormat, m_Width, m_Height, 0, srcFormat, GL_UNSIGNED_BYTE, data);

	//Tell the GPU to Generate Mipmaps
	if (generateMipmaps)
		glGenerateMipmap(GL_TEXTURE_2D);

	//Select Texture Filtering Options
	//SetTextureWrapAndFilter2D(filter);


	//glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, srcFormat, GL_UNSIGNED_BYTE, data);


	stbi_image_free(data);
	//glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::InitCubemapTexture(const std::string path[6])
{
	FR_CORE_ASSERT(!m_RendererID, "Texture already initiallized!");


	unsigned char* data;
	int width, height, numChannels;

	//Get Width and height
	data = stbi_load(path[0].c_str(), &width, &height, &numChannels, 4);
	stbi_image_free(data);

	//Apparently, On Cubemaps, textures are flipped upside down when using this, so we disable it for the moment
	stbi_set_flip_vertically_on_load(false);

	m_Width = width;
	m_Height = height;

	//Create OpenGLTexture of the appropriate type and Bind it
	glGenTextures(1, &m_RendererID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_RendererID);
	m_TextureType = TEXTURE_TYPE::CUBEMAP;

	
	//Load Textures' Data From Disk
	//Load textures one by one then upload it to the GPU
	for (int i = 0; i < NUM_CUBEMAP_FACES; i++)
	{
		data = stbi_load(path[i].c_str(), &width, &height, &numChannels, 3);
		FR_CORE_ASSERT(data, "Failed to load texture at : " << path[i]);
	
		//We currently only support RGB and RGBA textures
		GLenum srcFormat, gpuFormat;
		GetTextureSrcAndGPUFormat(srcFormat, gpuFormat, numChannels, true);

		glTexImage2D(
			GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
			0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
		);

		stbi_image_free(data);
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	//Reenable Flipping on load
	stbi_set_flip_vertically_on_load(true);
	//glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}


//TODO:  Create a shaderpool
void Texture::InitEquirectangularMap(const std::string& path, Texture& radianceTexture, Texture& prefilterMapTexture, Texture& brdfLUTTexture)
{
	//From https://learnopengl.com/PBR/IBL/Diffuse-irradiance
	
	FR_CORE_ASSERT(!m_RendererID, "Texture already initiallized!");
	//FR_CORE_ASSERT(radianceTexture, "Radiance Texture is null");
	FR_CORE_ASSERT(!radianceTexture.m_RendererID, "Irradiance texture is already initialized");

	m_TextureType = TEXTURE_TYPE::CUBEMAP;

	uint32_t captureFBO;
	uint32_t captureRBO;
	glGenFramebuffers(1, &captureFBO);
	glGenRenderbuffers(1, &captureRBO);

	glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
	glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 512, 512);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, captureRBO);
	
	// pbr: load the HDR environment map
	// ---------------------------------
	stbi_set_flip_vertically_on_load(true);
	int width, height, nrComponents;
	float* data = stbi_loadf(path.c_str(), &width, &height, &nrComponents, 0);
	unsigned int hdrTexture;
	FR_CORE_ASSERT(data, "Failed to load Equirectangular Map at " << path);

	glGenTextures(1, &hdrTexture);
	glBindTexture(GL_TEXTURE_2D, hdrTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, data); // note how we specify the texture's data value to be float

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(data);




	// pbr: setup cubemap to render to and attach to framebuffer
	// ---------------------------------------------------------
	unsigned int envCubemap;
	glGenTextures(1, &envCubemap);
	glBindTexture(GL_TEXTURE_CUBE_MAP, envCubemap);
	for (unsigned int i = 0; i < 6; ++i)
	{
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, 512, 512, 0, GL_RGB, GL_FLOAT, nullptr);
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// pbr: set up projection and view matrices for capturing data onto the 6 cubemap face directions
	// ----------------------------------------------------------------------------------------------
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

	// pbr: convert HDR equirectangular environment map to cubemap equivalent
	// ----------------------------------------------------------------------
	Shader equirectangularToCubemapShader("Assets/Shaders/Core/EquirectangularToCubemapShader.vert", "Assets/Shaders/Core/EquirectangularToCubemapShader.frag");
	equirectangularToCubemapShader.Use();
	equirectangularToCubemapShader.SetInt("equirectangularMap", 0);
	equirectangularToCubemapShader.SetMat4("projection", captureProjection);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, hdrTexture);

	Mesh cubemap;
	Mesh::InitMeshCubemap(cubemap);
	//cubemap.DiffuseTexID = envCubemap;

	glViewport(0, 0, 512, 512); // don't forget to configure the viewport to the capture dimensions.
	glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
	for (unsigned int i = 0; i < 6; ++i)
	{
		equirectangularToCubemapShader.SetMat4("view", captureViews[i]);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, envCubemap, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		cubemap.RenderMesh(equirectangularToCubemapShader);
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	m_RendererID = envCubemap;

	// then let OpenGL generate mipmaps from first mip face (combatting visible dots artifact)
	glBindTexture(GL_TEXTURE_CUBE_MAP, envCubemap);
	glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

	// Irradiance Texture


	unsigned int irradianceMap;
	glGenTextures(1, &irradianceMap);
	glBindTexture(GL_TEXTURE_CUBE_MAP, irradianceMap);
	for (unsigned int i = 0; i < 6; ++i)
	{
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, 32, 32, 0, GL_RGB, GL_FLOAT, nullptr);
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
	glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 32, 32);

	// pbr: solve diffuse integral by convolution to create an irradiance (cube)map.
	// -----------------------------------------------------------------------------
	Shader irradianceShader("Assets/Shaders/Core/EquirectangularToCubemapShader.vert", "Assets/Shaders/Core/DiffuseIrradianceShader.frag");
	irradianceShader.Use();
	irradianceShader.SetInt("environmentMap", 0);
	irradianceShader.SetMat4("projection", captureProjection);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, envCubemap);

	glViewport(0, 0, 32, 32); // don't forget to configure the viewport to the capture dimensions.
	glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
	for (unsigned int i = 0; i < 6; ++i)
	{
		irradianceShader.SetMat4("view", captureViews[i]);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, irradianceMap, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		cubemap.RenderMesh(irradianceShader);
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	radianceTexture.m_RendererID = irradianceMap;
	radianceTexture.m_Width = 32;
	radianceTexture.m_Height = 32;






	//Prefiltering
	
	
	unsigned int prefilterMap;
	glGenTextures(1, &prefilterMap);
	glBindTexture(GL_TEXTURE_CUBE_MAP, prefilterMap);
	for (unsigned int i = 0; i < 6; ++i)
	{
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, 128, 128, 0, GL_RGB, GL_FLOAT, nullptr);
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// generate mipmaps for the cubemap so OpenGL automatically allocates the required memory.
	glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

	Shader prefilterShader("Assets/Shaders/Core/EquirectangularToCubemapShader.vert", "Assets/Shaders/Core/prefilterShader.frag");

	prefilterShader.Use();
	prefilterShader.SetInt("environmentMap", 0);
	prefilterShader.SetMat4("projection", captureProjection);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, envCubemap);

	glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
	unsigned int maxMipLevels = 5;
	for (unsigned int mip = 0; mip < maxMipLevels; ++mip)
	{
		// reisze framebuffer according to mip-level size.
		unsigned int mipWidth = 128 * std::pow(0.5, mip);
		unsigned int mipHeight = 128 * std::pow(0.5, mip);
		glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, mipWidth, mipHeight);
		glViewport(0, 0, mipWidth, mipHeight);

		float roughness = (float)mip / (float)(maxMipLevels - 1);
		prefilterShader.SetFloat("roughness", roughness);
		for (unsigned int mip = 0; mip < maxMipLevels; ++mip)
		{
			// reisze framebuffer according to mip-level size.
			unsigned int mipWidth = static_cast<unsigned int>(128 * std::pow(0.5, mip));
			unsigned int mipHeight = static_cast<unsigned int>(128 * std::pow(0.5, mip));
			glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
			glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, mipWidth, mipHeight);
			glViewport(0, 0, mipWidth, mipHeight);

			float roughness = (float)mip / (float)(maxMipLevels - 1);
			prefilterShader.SetFloat("roughness", roughness);
			for (unsigned int i = 0; i < 6; ++i)
			{
				prefilterShader.SetMat4("view", captureViews[i]);
				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, prefilterMap, mip);

				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
				cubemap.RenderMesh(prefilterShader);
			}
		}
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	//Pbr lookup texture
	uint32_t brdfLUT;
	glGenTextures(1, &brdfLUT);
	glBindTexture(GL_TEXTURE_2D, brdfLUT);

	//Preallocate data
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RG16F, 512, 512, 0, GL_RG, GL_FLOAT, 0);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// then re-configure capture framebuffer object and render screen-space quad with BRDF shader.
	glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
	glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 512, 512);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, brdfLUT, 0);

	glViewport(0, 0, 512, 512);
	Shader brdfShader("Assets/Shaders/Core/brdfShader.vert", "Assets/Shaders/Core/brdfShader.frag");
	brdfShader.Use();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	Mesh quadMesh;
	Mesh::InitMeshFullScreenQuad(quadMesh);
	quadMesh.RenderMesh(brdfShader);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);


	prefilterMapTexture.m_RendererID = prefilterMap;
	brdfLUTTexture.m_RendererID = brdfLUT;

	Mesh::CleanUpMesh(cubemap);
	Mesh::CleanUpMesh(quadMesh);

	Window* window = Application::Get().GetWindow();
	

	glViewport(0, 0, window->GetWidth(), window->GetHeight());
}