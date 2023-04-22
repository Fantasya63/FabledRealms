#include "frpch.h"
#include "VertexArray.h"
#include <glad/glad.h>

VertexArray::VertexArray()
{
	glCreateVertexArrays(1, &m_RendererID);
}

VertexArray* VertexArray::Create()
{
	return new VertexArray();
}

void VertexArray::Bind()
{
	glBindVertexArray(m_RendererID);
}
