#pragma once
#include "Engine/Graphics/Rendering/FrameBuffer.h"

class ShadowFBO : public FrameBuffer
{
public:
	ShadowFBO();
	~ShadowFBO();

	void Init(uint32_t width, uint32_t height, int numShadowCascades);

	int m_NumShadowCascades = 3;

	void BindUniformUBO() const;
	void UnBindUniformUBO() const;

	uint32_t m_MatricesUBO;
};

