#pragma once

#include "Engine/Application.h"
#include "Engine/Graphics/Buffers.h"
#include "Engine/Graphics/VertexArray.h"
#include "Engine/Graphics/Shader.h"

#include "Engine/Time.h"

class FabledRealmsApp : public Application
{
public:
	FabledRealmsApp();
	~FabledRealmsApp();

	void OnUpdate(const Time& time) override;

private:
	VertexArray* m_HelloTriangleVAO;
	VertexBuffer* m_HelloTriangleVBO;
	IndexBuffer* m_HelloTriangleIBO;

	Shader* m_Shader;
};
