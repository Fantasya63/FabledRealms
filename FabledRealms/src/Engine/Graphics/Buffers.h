#pragma once

class VertexBuffer
{
public:
	~VertexBuffer();

	static VertexBuffer* Create(float* vertices, uint32_t size);
	void Bind();

private:
	VertexBuffer(float* vertices, uint32_t size);

private:
	uint32_t m_RendererID;
	float* m_Vertices;
};

// ------------------------------------------------


class IndexBuffer
{
public:
	~IndexBuffer();

	static IndexBuffer* Create(uint32_t* indices, uint32_t count);

	virtual uint32_t GetCount() const { return m_Count; }
	void Bind() const;

private:
	IndexBuffer(uint32_t* vertices, uint32_t count);

private:
	uint32_t m_RendererID;
	uint32_t m_Count;

	uint32_t* m_Indices;
};
