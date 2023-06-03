#include "frpch.h"
#include "FrameBuffer.h"
#include "glad/glad.h"

#include "Engine/Application.h"

FrameBuffer::FrameBuffer(bool addDepthAndStencil)
{
	glCreateFramebuffers(1, &m_RendererID);
	glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);

	//Add Depth And Stencil Attachment
	if (addDepthAndStencil)
	{
		//Hmmm?
		glCreateRenderbuffers(1, &m_DepthStencilAttachmentID);
		glBindRenderbuffer(GL_RENDERBUFFER, m_DepthStencilAttachmentID);

		Window* window = Application::Get().GetWindow();
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, window->GetWidth(), window->GetHeight());

		//Attach to FBO
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_DepthStencilAttachmentID);

	}

	//Unbind
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

FrameBuffer::~FrameBuffer()
{
	if (m_ColorAttachmentID)
		glDeleteTextures(1, &m_ColorAttachmentID);


	glDeleteFramebuffers(1, &m_RendererID);
}

void FrameBuffer::Bind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);

}

void FrameBuffer::UnBind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FrameBuffer::AddColorAttachment(bool isHDR)
{
	//Bind
	glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);

	// Create color texture
	glCreateTextures(GL_TEXTURE_2D, 1, &m_ColorAttachmentID);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_ColorAttachmentID);

	Window* window = Application::Get().GetWindow();

	if (isHDR)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, window->GetWidth(), window->GetHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	else
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, window->GetWidth(), window->GetHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);


	//Add it to the FrameBuffer
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_ColorAttachmentID, 0);

	//UnBind
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

const uint32_t FrameBuffer::GetColorAttachmentID() const
{
	return m_ColorAttachmentID;
}
