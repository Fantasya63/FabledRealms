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
	

	// Takes in an array of string as paths
	// For a normal Texture2D, it will only take the first element as the path
	// For a cubemap Texture, it will take the 6 elements as the path;
	Texture(const char texturePaths[6][100], TEXTURE_TYPE type, TEXTURE_FILTER filter);
	~Texture();

	void Bind() const;

	const TEXTURE_TYPE GetTextureType() const { return m_TextureType; }
	const uint32_t GetRendererID() const { return m_RendererID; }

private:
	void SetTextureWrapAndFilter2D(TEXTURE_FILTER filter);
	void SetTextureWrapAndFilterCubemap(TEXTURE_FILTER filter);

private:
	
	uint32_t m_RendererID = 0;
	TEXTURE_TYPE m_TextureType;
};

