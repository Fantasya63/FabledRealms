#pragma once
#include <glm/glm.hpp>
#include <vector>
#include <Engine/Graphics/Shader.h>
#include <Engine/Graphics/VertexArray.h>

struct BloomMip
{
	glm::vec2 TexSize;
	glm::ivec2 TexIntSize;
	uint32_t RendererID;
};

class BloomFBO
{
public:
	BloomFBO();
	~BloomFBO();

	bool Init(uint32_t width, uint32_t height, uint32_t mipChainLength);
	void Destroy();
	void Bind();
	const std::vector<BloomMip>& MipChain() const;

	void RenderBloomTexture(VertexArray& screenQuad, uint32_t srcTexture, float filterRadius);
	uint32_t GetBloomTexture() { return m_MipChain[0].RendererID; }
	

private:
	void RenderDownsamples(uint32_t srcTexture, VertexArray& screenQuad);
	void RenderUpsamples(float filterRadius, VertexArray& screenQuad);

private:

	glm::vec2 m_Resolution;

	Shader* m_DownsampleShader;
	Shader* m_UpsampleShader;

	bool m_IsInitialized;
	uint32_t m_RendererID;
	std::vector<BloomMip> m_MipChain;
};
