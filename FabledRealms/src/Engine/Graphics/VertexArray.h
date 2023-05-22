#pragma once
#include "Engine/Graphics/Buffers.h"

class VertexArray
{
public:
	~VertexArray() {};

	static VertexArray* Create();

	void Bind();

private:
	VertexArray(); // Not required, but done to keep the api consistent

private:
	uint32_t m_RendererID;
};

