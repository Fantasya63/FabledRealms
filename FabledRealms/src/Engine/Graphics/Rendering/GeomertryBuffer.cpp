#include "frpch.h"
#include "GeomertryBuffer.h"

GeomertryBuffer::GeomertryBuffer()
{
}

GeomertryBuffer::~GeomertryBuffer()
{
}

//TODO: Calculate position based on scene depth
void GeomertryBuffer::Init(uint32_t width, uint32_t height)
{
	FrameBuffer::Init(width, height);

	//Position and emission
	FrameBuffer::AddColorAttachment(width, height, ColorFormat::RGBA16F);
	
	//Color and metalic
	FrameBuffer::AddColorAttachment(width, height, ColorFormat::RGBA);
	
	//Normal, Roughness
	FrameBuffer::AddColorAttachment(width, height, ColorFormat::RGBA16F);

	//Tell the GPU about our attachments
	FrameBuffer::SetDrawBuffers();
}
