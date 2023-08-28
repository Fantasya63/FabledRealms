#pragma once
#include <Engine/Graphics/Rendering/FrameBuffer.h>

class TransparentFBO : public FrameBuffer
{
public:
	TransparentFBO();
	~TransparentFBO();

	virtual void Init(uint32_t width, uint32_t height) override;
};

