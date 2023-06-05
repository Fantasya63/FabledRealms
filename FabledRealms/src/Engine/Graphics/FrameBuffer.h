#pragma once
#include "Engine/Graphics/Texture.h"
#include "Engine/Graphics/VertexArray.h"

#include <vector>
#include <glm/glm.hpp>

class FrameBuffer
{
public:

	FrameBuffer();
	~FrameBuffer();

	void Bind();
	void UnBind();


	enum class ColorFormat
	{
		RGB = 0,
		RGB16F,
		R11F_G11F_B10F,
	};
	void AddColorAttachment(uint32_t width, uint32_t height, ColorFormat format);
	void AddDepthStencilAttachment(uint32_t width, uint32_t height);


	const uint32_t GetColorAttachmentID(uint32_t index) const;
	const glm::ivec2 GetResolution() const;

protected:
	virtual void Init(uint32_t width, uint32_t height);
	

protected:
	glm::ivec2 m_Resolution;
	uint32_t m_RendererID;

private:
	bool m_IsInitialized = false;
	std::vector<uint32_t> m_ColorAttachmentIDs;

	//uint32_t m_ColorAttachmentID;
	uint32_t m_DepthStencilAttachmentID;
};