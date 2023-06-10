#pragma once
#include <glm/glm.hpp>
#include <vector>
#include <Engine/Graphics/Shader.h>
#include <Engine/Graphics/Mesh/Mesh.h>

#include <Engine/Graphics/Rendering/FrameBuffer.h>


struct BloomMip
{
	glm::vec2 TexSize;
	glm::ivec2 TexIntSize;
	uint32_t RendererID;
};


class BloomFBO : FrameBuffer
{
public:
	BloomFBO();
	~BloomFBO();

	void Init(uint32_t width, uint32_t height, uint32_t mipChainLength);

	const std::vector<BloomMip>& MipChain() const;

	void RenderBloomTexture(uint32_t srcTexture, float filterRadius);
	const uint32_t GetBloomTexture() const { return m_MipChain[0].RendererID; }

private:
	void RenderDownsamples(uint32_t srcTexture);
	void RenderUpsamples(float filterRadius);
	void RenderClippedHDR(uint32_t srcTex);
	
private:
	Mesh m_ScreenQuad;

	uint32_t m_ClipTexture;
	Shader* m_ClipShader;
	Shader* m_DownsampleShader;
	Shader* m_UpsampleShader;
	std::vector<BloomMip> m_MipChain;
};
