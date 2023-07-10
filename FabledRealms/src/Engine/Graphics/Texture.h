#pragma once
class Texture
{
public:
	enum class TEXTURE_TYPE
	{
		TEXTURE2D,
		CUBEMAP,
	};

	enum class TEXTURE_FILTER
	{
		 NEAREST = 0,
		 LINEAR,
	};
	
	Texture() {};
	~Texture();

	void InitTexture2D(const std::string& path, TEXTURE_FILTER filter, bool isColorData = true, bool generateMipmaps = false, bool repeat = true);
	void InitCubemapTexture(const std::string path[6]);
	void Texture::InitEquirectangularMap(const std::string& path, Texture& radianceTexture, Texture& prefilterMapTexture, Texture& brdfLUTTexture);



	void Bind(uint32_t slot = 0) const;

	const TEXTURE_TYPE GetTextureType() const { return m_TextureType; }
	const uint32_t GetRendererID() const { return m_RendererID; }

private:
	void SetTextureWrapAndFilter2D(TEXTURE_FILTER filter);
	void SetTextureWrapAndFilterCubemap(TEXTURE_FILTER filter);

private:
	uint32_t m_Width, m_Height;

	uint32_t m_RendererID = 0;
	TEXTURE_TYPE m_TextureType;
};