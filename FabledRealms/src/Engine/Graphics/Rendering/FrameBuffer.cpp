#include "frpch.h"
#include "FrameBuffer.h"
#include "glad/glad.h"

#include "Engine/Application.h"


FrameBuffer::FrameBuffer()
{
	glCreateFramebuffers(1, &m_RendererID);
}

FrameBuffer::~FrameBuffer()
{
	glDeleteFramebuffers(1, &m_RendererID);
}

void FrameBuffer::Init(uint32_t width, uint32_t height)
{
	if (m_IsInitialized)
	{
		FR_CORE_ASSERT(false, "Framebuffer is already initialized!");
		return;
	}

	m_Resolution = glm::ivec2(width, height);
	m_IsInitialized = true;
}

void FrameBuffer::SetDrawBuffers()
{
	std::vector<GLenum> buffers;

	//Populate buffers
	int count = m_ColorAttachmentIDs.size();
	for (int i = 0; i < count; i++)
	{
		buffers.emplace_back(GL_COLOR_ATTACHMENT0 + i);
	}

	//Tell opengl which attachments we are outputing to
	glDrawBuffers(count, buffers.data());
}

void FrameBuffer::Bind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);
}

void FrameBuffer::UnBind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FrameBuffer::AddColorAttachment(uint32_t width, uint32_t height, ColorFormat format)
{
	GLenum colorFormat;
	switch (format)
	{
	case ColorFormat::RGB:
		colorFormat = GL_RGB;
		break;

	case ColorFormat::RGBA:
		colorFormat = GL_RGBA;
		break;

	case ColorFormat::RGB16F:
		colorFormat = GL_RGB16F;
		break;

	case ColorFormat::RGBA16F:
		colorFormat = GL_RGBA16F;
		break;

	case ColorFormat::R11F_G11F_B10F:
		colorFormat = GL_R11F_G11F_B10F;
		break;

	default:
		FR_CORE_ASSERT(false, "Unknown Framebuffer color attachment format");
		return;
	}

	// Bind Framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);

	// Create color texture
	uint32_t id;
	glCreateTextures(GL_TEXTURE_2D, 1, &id);
	glActiveTexture(GL_TEXTURE0 + m_ColorAttachmentIDs.size());
	glBindTexture(GL_TEXTURE_2D, id);

	
	//Allocate Data
	glTexImage2D(GL_TEXTURE_2D, 0, colorFormat, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

	//Set Filter and wrapping options
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);


	//Add it to the FrameBuffer
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + m_ColorAttachmentIDs.size(), GL_TEXTURE_2D, id, 0);

	m_ColorAttachmentIDs.emplace_back(id);

	//UnBind
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FrameBuffer::AddDepthStencilAttachment(uint32_t width, uint32_t height)
{
	//Bind
	glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);

	
	glCreateRenderbuffers(1, &m_DepthStencilAttachmentID);
	glBindRenderbuffer(GL_RENDERBUFFER, m_DepthStencilAttachmentID);

	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);

	//Attach to FBO
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_DepthStencilAttachmentID);
	
	//Unbind
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

const uint32_t FrameBuffer::GetColorAttachmentID(uint32_t index) const
{
	FR_CORE_ASSERT(m_ColorAttachmentIDs.size(), "Color AttachmentIDs is empty!");
	return m_ColorAttachmentIDs[index];
}

const glm::ivec2 FrameBuffer::GetResolution() const
{
	return  m_Resolution;
}
