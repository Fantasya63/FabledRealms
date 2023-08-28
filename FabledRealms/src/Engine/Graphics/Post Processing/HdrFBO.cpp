#include "frpch.h"
#include "HdrFBO.h"
#include "Engine/Application.h"

#include <glad/glad.h>

HdrFBO::HdrFBO()
{
}

HdrFBO::~HdrFBO()
{
	//glDeleteTextures(1, &m_DepthStencilAttachmentID);
}

void HdrFBO::Init(uint32_t width, uint32_t height)
{
	FrameBuffer::Init(width, height);
	FrameBuffer::Bind();

	AddColorAttachment(width, height, ColorFormat::RGB16F);
	AddDepthAttachment(width, height);
}