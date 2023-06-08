#pragma once
#include <Engine/Graphics/Rendering/FrameBuffer.h>


class HdrFBO : public FrameBuffer
{
public:
	HdrFBO();
	~HdrFBO();

	virtual void Init(uint32_t width, uint32_t height) override;
};

