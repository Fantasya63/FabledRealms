#pragma once
#include <Engine/Graphics/Rendering/FrameBuffer.h>

class SSRBlurFBO : public FrameBuffer
{
public:
	SSRBlurFBO();
	~SSRBlurFBO();

	virtual void Init(uint32_t width, uint32_t height) override;
};

