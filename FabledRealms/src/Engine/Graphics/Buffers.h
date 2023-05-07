#pragma once

class VertexBuffer
{
public:
	~VertexBuffer();

	static VertexBuffer* Create(float* vertices, uint32_t size);
	void Bind();
	

private:
	VertexBuffer(float* vertices, uint32_t size);
	void SetVertexLayout();

private:
	uint32_t m_RendererID;
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
};
