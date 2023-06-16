#include "frpch.h"
#include "BloomFBO.h"
#include <glad/glad.h>

BloomFBO::BloomFBO()
{
    m_ClipShader = new Shader("Assets/Shaders/Post Processing/Bloom/BloomShader.vert", "Assets/Shaders/Post Processing/Bloom/BloomClip.frag");
    m_DownsampleShader = new Shader("Assets/Shaders/Post Processing/Bloom/BloomShader.vert", "Assets/Shaders/Post Processing/Bloom/BloomDownsampleShader.frag");
    m_UpsampleShader = new Shader("Assets/Shaders/Post Processing/Bloom/BloomShader.vert", "Assets/Shaders/Post Processing/Bloom/BloomUpsampleShader.frag");
}

BloomFBO::~BloomFBO()
{
    Mesh::CleanUpMesh(m_ScreenQuad);

    //Delete Clip Texture
    glDeleteTextures(1, &m_ClipTexture);

    delete m_DownsampleShader;
    delete m_UpsampleShader;
    delete m_ClipShader;
}

void BloomFBO::Init(uint32_t width, uint32_t height, uint32_t mipChainLength)
{
    FrameBuffer::Init(width, height);

    glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);

    Mesh::InitMeshFullScreenQuad(m_ScreenQuad);


    glm::vec2 mipSize((float)width, (float)height);
    glm::ivec2 mipIntSize(width, height);

    //Safety Check
    if (width > (uint32_t)INT_MAX || height > (uint32_t)INT_MAX) {
        LOG_CORE_ERROR("Window size conversion overflow! Failed to Build BloomFBO!");
        return;
    }


    //Create texture for clipping
    glGenTextures(1, &m_ClipTexture);

    glBindTexture(GL_TEXTURE_2D, m_ClipTexture);

    //Create float texture format and downscale
    glTexImage2D(GL_TEXTURE_2D, 0, GL_R11F_G11F_B10F,
        width, height,
        0, GL_RGB, GL_FLOAT, nullptr);

    //Set texture filter and wrapping
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    //Create mip textures
    for (int i = 0; i < mipChainLength; i++)
    {
        BloomMip mip;

        mipSize *= 0.5f;
        mipIntSize /= 2;

        mip.TexSize = mipSize;
        mip.TexIntSize = mipIntSize;

        //Create Texture
        glGenTextures(1, &mip.RendererID);
        glBindTexture(GL_TEXTURE_2D, mip.RendererID);

        //Create float texture format and downscale
        glTexImage2D(GL_TEXTURE_2D, 0, GL_R11F_G11F_B10F,
            (int)mipSize.x, (int)mipSize.y,
            0, GL_RGB, GL_FLOAT, nullptr);

        //Set texture filter and wrapping
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        m_MipChain.emplace_back(mip);
    }

    // Attach a texture to the frame buffer
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
        GL_TEXTURE_2D, m_MipChain[0].RendererID, 0);

    //Setup Attachments
    uint32_t attachments[1] = { GL_COLOR_ATTACHMENT0 };
    glDrawBuffers(1, attachments);

    //Check Completion status
    int status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (status != GL_FRAMEBUFFER_COMPLETE)
    {
        FR_CORE_ASSERT(false, "BloomFBO Completion status is not met!");
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        return;
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    return;
}


const std::vector<BloomMip>& BloomFBO::MipChain() const
{
    return m_MipChain;
}

void BloomFBO::RenderBloomTexture(uint32_t srcTexture, float filterRadius)
{
    //Bind Frame buffer
    glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);
    glBindVertexArray(m_ScreenQuad.VAO);

    //Clip HDR
    RenderClippedHDR(srcTexture);

    //Render DownSamples
    RenderDownsamples(m_ClipTexture);

    //Render Upsamples
    RenderUpsamples(filterRadius);

    //Unbind
    glBindVertexArray(0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    //Restore Viewport
    glViewport(0, 0, m_Resolution.x, m_Resolution.y);
}

void BloomFBO::RenderDownsamples(uint32_t srcTexture)
{
    //screenQuad.Bind();
    m_DownsampleShader->Use();
    m_DownsampleShader->SetVec2("srcResolution", m_Resolution);
    m_DownsampleShader->SetInt("mipLevel", 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, srcTexture);

    //Progressively Downsample through the mip chain
    for (int i = 0; i < m_MipChain.size(); i++)
    {
        const BloomMip& mip = m_MipChain[i];
        //Set the viewport size to the mip's size
        glViewport(0, 0, mip.TexSize.x, mip.TexSize.y);

        //Set output  color attachment
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
            GL_TEXTURE_2D, mip.RendererID, 0);

        //Render screen-filled quad
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL);

        //Set current mip resolution as the srcResolution for the next iteration
        m_DownsampleShader->SetVec2("srcResolution", mip.TexSize);

        /// Set current mip as texture input for next iteration
        glBindTexture(GL_TEXTURE_2D, mip.RendererID);

        // Disable Karis average for consequent downsamples
        if (i == 0) { m_DownsampleShader->SetInt("mipLevel", 1); }
    }
}

void BloomFBO::RenderUpsamples(float filterRadius)
{
    m_UpsampleShader->Use();
    m_UpsampleShader->SetFloat("filterRadius", filterRadius);

    //Enable Additive blending
    glEnable(GL_BLEND);
    glBlendFunc(GL_ONE, GL_ONE);
    glBlendEquation(GL_FUNC_ADD);

    for (int i = (int)m_MipChain.size() - 1; i > 0; i--)
    {
        const BloomMip& mip = m_MipChain[i];
        const BloomMip& nextMip = m_MipChain[i - 1];

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, mip.RendererID);

        //Set viewport to the one we will be writing
        glViewport(0, 0, nextMip.TexSize.x, nextMip.TexSize.y);

        // set output of the frame buffer texture
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
            GL_TEXTURE_2D, nextMip.RendererID, 0);

        //Render quad-filled screen
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL);
    }

    //Disable Additive blending
    glDisable(GL_BLEND);
}

void BloomFBO::RenderClippedHDR(uint32_t srcTex)
{
    m_ClipShader->Use();
    m_ClipShader->SetInt("srcTexture", 0);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, srcTex);

    // set output of the frame buffer texture
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
        GL_TEXTURE_2D, m_ClipTexture, 0);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL);
}