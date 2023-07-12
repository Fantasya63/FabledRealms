#pragma once
#include <glm/ext/matrix_float4x4.hpp>
class Shader
{
public:
	Shader(const char* vertexPath, const char* fragmentPath, const char* geometryPath = nullptr);
	~Shader();

	void Use() const;

	void SetInt(const char* name, int value);
	void SetFloat(const char* name, float value);
	void SetVec2(const char* name, glm::vec2 value);
	void SetVec3(const char* name, glm::vec3 value);
	
	void SetMat3(const char* name, const glm::mat3& value);
	void SetMat4(const char* name, const glm::mat4& value);

	//Temporary
	inline uint32_t GetRendererID() { return m_RendererID; }

private:
	uint32_t m_RendererID;
};

