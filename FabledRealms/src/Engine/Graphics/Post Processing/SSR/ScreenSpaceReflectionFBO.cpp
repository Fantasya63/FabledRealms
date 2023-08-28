#include "frpch.h"
#include "ScreenSpaceReflectionFBO.h"

ScreenSpaceReflectionFBO::ScreenSpaceReflectionFBO()
{
}

ScreenSpaceReflectionFBO::~ScreenSpaceReflectionFBO()
{
}

void ScreenSpaceReflectionFBO::Init(uint32_t width, uint32_t height)
{
	FrameBuffer::Init(width, height);
	FrameBuffer::Bind();

	//UV Visibility
	AddColorAttachment(width, height, ColorFormat::RGB16F);

	//Blur
	AddColorAttachment(width, height, ColorFormat::RGB16F);
}
