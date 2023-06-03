#pragma once
#include "Engine/Graphics/Texture.h"

class FrameBuffer
{
public:

	FrameBuffer(bool addDepthAndStencil = true);
	~FrameBuffer();

	void Bind();
	void UnBind();

	void AddColorAttachment(bool isHDR);

	//Test
	void BindColorAttachment();

	const uint32_t GetColorAttachmentID() const;

private:
	uint32_t m_RendererID;
	uint32_t m_ColorAttachmentID;
	uint32_t m_DepthStencilAttachmentID;
};