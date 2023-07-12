#pragma once
#include <Engine/Graphics/Rendering/FrameBuffer.h>

class ScreenSpaceReflectionFBO : FrameBuffer
{
public:
	ScreenSpaceReflectionFBO();
	~ScreenSpaceReflectionFBO();

	virtual void Init(uint32_t width, uint32_t height) override;
};

