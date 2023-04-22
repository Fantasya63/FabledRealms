#pragma once

#include "Engine/Application.h"
#include "Engine/Graphics/Buffers.h"
#include "Engine/Graphics/VertexArray.h"

class FabledRealmsApp : public Application
{
public:
	FabledRealmsApp();
	~FabledRealmsApp();

	void OnUpdate(float deltaTime) override;

private:
	VertexArray* m_HelloTriangleVAO;
	VertexBuffer* m_HelloTriangleVBO;
	IndexBuffer* m_HelloTriangleIBO;

	uint32_t m_Shader;
	unsigned int VBO, VAO, EBO;
};
