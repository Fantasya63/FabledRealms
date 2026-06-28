#include "frpch.h"
#include "GeometryBuffer.h"
#include <glad/glad.h>

GeometryBuffer::GeometryBuffer()
{
	DLOG_CORE_INFO("GEOMETRY BUFFER CONSTRUCTOR");
}

GeometryBuffer::~GeometryBuffer()
{
	glDeleteTextures(1, &m_DepthStencilAttachmentID);
}

//TODO: Calculate position based on scene depth
void GeometryBuffer::Init(uint32_t width, uint32_t height)
{
	FrameBuffer::Init(width, height);
	FrameBuffer::Bind();

	//Position and emission
	FrameBuffer::AddColorAttachment(width, height, ColorFormat::RGBA16F);

	//Color and metalic
	FrameBuffer::AddColorAttachment(width, height, ColorFormat::RGBA);

	//Normal, Roughness
	FrameBuffer::AddColorAttachment(width, height, ColorFormat::RGBA16F);

	//Shadow, TexAO, Light
	//FrameBuffer::AddColorAttachment(width, height, ColorFormat::RGBA);

	//AddDepthAttachment(width, height);
	AddDepthAttachmentTexture(width, height, FrameBuffer::DepthStencilFormat::Depth24);

	//Tell the GPU about our attachments
	FrameBuffer::SetDrawBuffers();

	//Check Completion status
	FR_CORE_ASSERT(CheckIfComplete(), "GeometryBuffer Completion status is not met!");
}

