#include "frpch.h"
#include "Buffers.h"
#include <glad/glad.h>

// ----------------- VERTEX BUFFER ------------------------------------

VertexBuffer::VertexBuffer(float* vertices, uint32_t size)
{
	//Create a buffer in the gpu, this will also set the m_RendererID as the location of the data in the GPU (like pointers)
	glCreateBuffers(1, &m_RendererID);

	//Bind Buffer
	glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);

	//Upload the data to the GPU
	glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);

	SetVertexLayout();
}

VertexBuffer::~VertexBuffer()
{
	//Delete the data on the GPU
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

void VertexBuffer::SetVertexLayout()
{
	// - According to the OpenGl 

	//POSITION
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//NORMAL
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	//UV
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	//Ambient
}

// ---------------- INDEX BUFFER -----------------------------------

IndexBuffer::IndexBuffer(uint32_t* indices, uint32_t count)
{
	m_Count = count;
	
	glCreateBuffers(1, &m_RendererID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), indices, GL_STATIC_DRAW);
}

IndexBuffer::~IndexBuffer()
{
	glDeleteBuffers(1, (GLuint*)&m_RendererID);
}

IndexBuffer* IndexBuffer::Create(uint32_t* indices, uint32_t count)
{
	return new IndexBuffer(indices, count);
}

void IndexBuffer::Bind() const
{
}
