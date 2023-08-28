#include "frpch.h"
#include "ShadowFBO.h"
#include "glad/glad.h"

ShadowFBO::ShadowFBO()
{
}

ShadowFBO::~ShadowFBO()
{
	glDeleteBuffers(1, &m_MatricesUBO);
}

void ShadowFBO::Init(uint32_t width, uint32_t height, int numShadowCascades)
{
	FrameBuffer::Init(width, height);

	m_NumShadowCascades = numShadowCascades;

	glGenTextures(1, &m_DepthStencilAttachmentID);
	glBindTexture(GL_TEXTURE_2D_ARRAY, m_DepthStencilAttachmentID);

	glTexImage3D(
		GL_TEXTURE_2D_ARRAY,
		0,
		GL_DEPTH_COMPONENT32F,
		width,
		height,
		m_NumShadowCascades + 1,
		0,
		GL_DEPTH_COMPONENT,
		GL_FLOAT,
		nullptr
	);


	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

	constexpr float bordercolor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glTexParameterfv(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_BORDER_COLOR, bordercolor);

	glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, m_DepthStencilAttachmentID, 0);

	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);

	int status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	FR_CORE_ASSERT(status == GL_FRAMEBUFFER_COMPLETE, "Shadow FBO is not Complete!");

	glBindFramebuffer(GL_FRAMEBUFFER, 0);


	//Configure UBO
	glGenBuffers(1, &m_MatricesUBO);
	glBindBuffer(GL_UNIFORM_BUFFER, m_MatricesUBO);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::mat4) * 16, nullptr, GL_STATIC_DRAW);
	glBindBufferBase(GL_UNIFORM_BUFFER, 0, m_MatricesUBO);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void ShadowFBO::BindUniformUBO() const
{
	glBindBuffer(GL_UNIFORM_BUFFER, m_MatricesUBO);
}

void ShadowFBO::UnBindUniformUBO() const
{
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}
