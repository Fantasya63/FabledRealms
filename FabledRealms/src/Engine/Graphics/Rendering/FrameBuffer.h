#pragma once
#include "Engine/Graphics/Texture.h"

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
		RGBA,
		RGB16F,
		RGBA16F,
		R11F_G11F_B10F,
	};
	enum class DepthAttachmentType
	{
		None,
		Renderbuffer,
		Texture
	};

	enum class DepthStencilFormat
	{
		Depth24,
		Depth24_Stencil8
	};

	void AddColorAttachment(uint32_t width, uint32_t height, ColorFormat format);
	void AddDepthAttachment(uint32_t width, uint32_t height, DepthStencilFormat format);
	void AddDepthAttachmentTexture(uint32_t width, uint32_t height, DepthStencilFormat format);

	const uint32_t GetColorAttachmentID(uint32_t index) const;
	const uint32_t GetDepthAttachmentID() const;
	const uint32_t GetRendererID() const { return m_RendererID; };
	
	const glm::ivec2 GetResolution() const;

	bool CheckIfComplete() const;
	

protected:
	virtual void Init(uint32_t width, uint32_t height);
	
	// tell OpenGL which color attachments we'll use (of this framebuffer) for rendering 
	void SetDrawBuffers();

protected:
	glm::ivec2 m_Resolution;
	uint32_t m_RendererID;
	uint32_t m_DepthStencilAttachmentID = 0;


private:

	bool m_IsInitialized = false;
	DepthAttachmentType m_DepthAttachmentType = DepthAttachmentType::None;
	std::vector<uint32_t> m_ColorAttachmentIDs;
};