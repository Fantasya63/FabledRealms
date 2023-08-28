#include "frpch.h"
#include "SSRBlurFBO.h"

SSRBlurFBO::SSRBlurFBO()
{
}

SSRBlurFBO::~SSRBlurFBO()
{
}

void SSRBlurFBO::Init(uint32_t width, uint32_t height)
{
	FrameBuffer::Init(width, height);
	FrameBuffer::Bind();

	//UV Visibility
	AddColorAttachment(width, height, ColorFormat::RGB16F);

	//Blur
	AddColorAttachment(width, height, ColorFormat::RGB16F);
}
