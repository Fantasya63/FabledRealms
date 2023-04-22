#include "frpch.h"
#include "Buffers.h"
#include <glad/glad.h>

// ----------------- VERTEX BUFFER ------------------------------------

VertexBuffer::VertexBuffer(float* vertices, uint32_t size)
{
	m_Vertices = vertices;

	glCreateBuffers(1, &m_RendererID);
	glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
	glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
}

VertexBuffer::~VertexBuffer()
{
	delete[] m_Vertices;
	glDeleteBuffers(1, &m_RendererID);
}

VertexBuffer* VertexBuffer::Create(float* vertices, uint32_t size)
{
	return new VertexBuffer(vertices, size);
}

void VertexBuffer::Bind()
{
	glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
}

// ---------------- INDEX BUFFER -----------------------------------

IndexBuffer::IndexBuffer(uint32_t* indices, uint32_t count)
{
	m_Count = count;
	m_Indices = indices;

	glCreateBuffers(1, &m_RendererID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), indices, GL_STATIC_DRAW);
}

IndexBuffer::~IndexBuffer()
{
	delete[] m_Indices;
	glDeleteBuffers(1, (GLuint*)&m_RendererID);
}

IndexBuffer* IndexBuffer::Create(uint32_t* indices, uint32_t count)
{
	return new IndexBuffer(indices, count);
}

void IndexBuffer::Bind() const
{
}
