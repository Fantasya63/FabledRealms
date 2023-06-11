#pragma once
#include <Engine/Graphics/Rendering/FrameBuffer.h>

class GeometryBuffer : public FrameBuffer
{
public:
	GeometryBuffer();
	~GeometryBuffer();

	void Init(uint32_t width, uint32_t height);
};

