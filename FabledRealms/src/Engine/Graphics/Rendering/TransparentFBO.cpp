#include "frpch.h"
#include "TransparentFBO.h"

TransparentFBO::TransparentFBO()
{
}

TransparentFBO::~TransparentFBO()
{
}

void TransparentFBO::Init(uint32_t width, uint32_t height)
{
	FrameBuffer::Init(width, height);
	FrameBuffer::Bind();

	AddColorAttachment(width, height, ColorFormat::RGB16F);
	AddDepthAttachment(width, height);
}
