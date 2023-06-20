#include "frpch.h"
#include "HdrFBO.h"
#include "Engine/Application.h"

HdrFBO::HdrFBO()
{
}

HdrFBO::~HdrFBO()
{
}

void HdrFBO::Init(uint32_t width, uint32_t height)
{
	FrameBuffer::Init(width, height);
	FrameBuffer::Bind();

	AddColorAttachment(width, height, ColorFormat::RGB16F);
	AddDepthAttachment(width, height);
}