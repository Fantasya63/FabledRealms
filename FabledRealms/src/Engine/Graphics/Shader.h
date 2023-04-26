#pragma once
class Shader
{
public:
	Shader(const char* vertexPath, const char* fragmentPath);
	~Shader();

	void Use() const;

	void SetFloat(const char* name, float value);

	//Temporary
	inline uint32_t GetRendererID() { return m_RendererID; }

private:
	uint32_t m_RendererID;
};

