#include "frpch.h"
#include "VertexArray.h"
#include <glad/glad.h>

VertexArray::VertexArray()
{
	//Create a vertex array on the GPU and the m_RendererID will be given a value by Opengl
	glCreateVertexArrays(1, &m_RendererID);
}

VertexArray* VertexArray::Create()
{
	return new VertexArray();
}

void VertexArray::Bind()
{
	//Bind the Vertex array
	glBindVertexArray(m_RendererID);
}

void VertexArray::UnBind()
{
	glBindVertexArray(0);
}
