#pragma once
#include "Engine/Graphics/Buffers.h"

class VertexArray
{
public:
	~VertexArray() {};

	static VertexArray* Create();

	void Bind();

	inline void SetVertexBuffer(VertexBuffer& vbo) { m_VertexBuffer = &vbo; };
	inline void SetIndexBuffer(IndexBuffer& ibo) { m_IndexBuffer = &ibo; };


private:
	VertexArray(); // Not required, but done to keep the api consistent

private:
	uint32_t m_RendererID;

	VertexBuffer* m_VertexBuffer;
	IndexBuffer* m_IndexBuffer;

};

