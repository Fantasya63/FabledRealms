#pragma once
#include <Engine/Graphics/Rendering/FrameBuffer.h>

class GeomertryBuffer : FrameBuffer
{
public:
	GeomertryBuffer();
	~GeomertryBuffer();

	void Init(uint32_t width, uint32_t height);
};

