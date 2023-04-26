#pragma once
class Texture
{
public:
	enum TEX_FILTERING
	{
		 NEAREST = 0,
		 LINEAR,
	};

	Texture(const char* texturePath, TEX_FILTERING filter);
	~Texture();

	void Bind() const;

private:
	int m_Width, m_Height, m_NumChannels;

	uint32_t m_RendererID;
};

