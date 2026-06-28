#include "frpch.h"
#include "FrameBuffer.h"
#include "glad/glad.h"

#include "Engine/Application.h"


FrameBuffer::FrameBuffer()
{
	DLOG_CORE_INFO("FrameBuffer base constructor");
	glCreateFramebuffers(1, &m_RendererID);
}

FrameBuffer::~FrameBuffer()
{
	glDeleteTextures(m_ColorAttachmentIDs.size(), m_ColorAttachmentIDs.data());
	
	if (m_DepthStencilAttachmentID)
	{
		switch (m_DepthAttachmentType)
		{
		case DepthAttachmentType::Renderbuffer:
			glDeleteRenderbuffers(1, &m_DepthStencilAttachmentID);
			break;

		case DepthAttachmentType::Texture:
			glDeleteTextures(1, &m_DepthStencilAttachmentID);
			break;

		default:
			break;
		}
	}


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
	//FrameBuffer::Bind();

	std::vector<GLenum> buffers;

	int count = m_ColorAttachmentIDs.size();
	for (int i = 0; i < count; i++)
	{
		buffers.emplace_back(GL_COLOR_ATTACHMENT0 + i);
	}
	
	glNamedFramebufferDrawBuffers(
		m_RendererID,
		count,
		buffers.data()
	);
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
		colorFormat = GL_RGB8;
		break;

	case ColorFormat::RGBA:
		colorFormat = GL_RGBA8;
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

	uint32_t id;
	glCreateTextures(GL_TEXTURE_2D, 1, &id);
	glTextureStorage2D(id, 1, colorFormat,
		width, height);

	glTextureParameteri(id, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTextureParameteri(id, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTextureParameteri(id, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTextureParameteri(id, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glNamedFramebufferTexture(
		m_RendererID,
		GL_COLOR_ATTACHMENT0 + m_ColorAttachmentIDs.size(),
		id,
		0
	);


	
	//Add it to the FrameBuffer
	m_ColorAttachmentIDs.emplace_back(id);
}



void FrameBuffer::AddDepthAttachment(uint32_t width, uint32_t height)
{
	FR_CORE_ASSERT(!m_DepthStencilAttachmentID, "Depth attachment already exist!");
	
	glCreateRenderbuffers(1, &m_DepthStencilAttachmentID);

	glNamedRenderbufferStorage(
		m_DepthStencilAttachmentID,
		GL_DEPTH24_STENCIL8,
		width, height
	);

	glNamedFramebufferRenderbuffer(
		m_RendererID,
		GL_DEPTH_STENCIL_ATTACHMENT,
		GL_RENDERBUFFER,
		m_DepthStencilAttachmentID
	);

	m_DepthAttachmentType = DepthAttachmentType::Renderbuffer;
}

void FrameBuffer::AddDepthAttachmentTexture(uint32_t width, uint32_t height)
{
	FR_CORE_ASSERT(!m_DepthStencilAttachmentID, "Depth attachment already exist!");
	
	glCreateTextures(GL_TEXTURE_2D, 1, &m_DepthStencilAttachmentID);
	glTextureStorage2D(
		m_DepthStencilAttachmentID,
		1,
		GL_DEPTH_COMPONENT24,
		width,
		height
	);

	glTextureParameteri(m_DepthStencilAttachmentID, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTextureParameteri(m_DepthStencilAttachmentID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTextureParameteri(m_DepthStencilAttachmentID, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTextureParameteri(m_DepthStencilAttachmentID, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glNamedFramebufferTexture(
		m_RendererID,
		GL_DEPTH_ATTACHMENT,
		m_DepthStencilAttachmentID,
		0
	);

	m_DepthAttachmentType = DepthAttachmentType::Texture;
}



const uint32_t FrameBuffer::GetColorAttachmentID(uint32_t index) const
{
	FR_CORE_ASSERT(m_ColorAttachmentIDs.size(), "Color AttachmentIDs is empty!");
	FR_CORE_ASSERT(index < m_ColorAttachmentIDs.size(), "Index out of range!");
	return m_ColorAttachmentIDs[index];
}

const uint32_t FrameBuffer::GetDepthAttachmentID() const
{
	return m_DepthStencilAttachmentID;
}



const glm::ivec2 FrameBuffer::GetResolution() const
{
	return  m_Resolution;
}
