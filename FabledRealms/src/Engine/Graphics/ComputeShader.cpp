#include "frpch.h"
#include "ComputeShader.h"
#include <glad/glad.h>
#include <fstream>
#include <sstream>

ComputeShader::ComputeShader(const std::string& path)
{
	std::string srcCode;
	
	std::ifstream shaderFile;
	

	// Make sure we can throw exceptions
	shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	
	try
	{
		shaderFile.open(path);

		std::stringstream ss;

		ss << shaderFile.rdbuf();
		srcCode = ss.str();

	}
	catch (std::ifstream::failure e)
	{
		LOG_CORE_ERROR("Failed To Read Vertex Shader at: " << path);
	}

	const char* cSourceCode = srcCode.c_str();
	int success = 0;

	uint32_t compute;
	compute = glCreateShader(GL_COMPUTE_SHADER);
	glShaderSource(compute, 1, &cSourceCode, NULL);
	glCompileShader(compute);
	
	//Check it's compilation status
	glGetShaderiv(compute, GL_COMPILE_STATUS, &success);

	//Check if we succesfully compiled the shader
	if (!success)
	{
		// Get error infos
		int maxLength = 0;
		glGetShaderiv(compute, GL_INFO_LOG_LENGTH, &maxLength);

		// Max length includes the null character
		char* infoLog = new char[maxLength];
		glGetShaderInfoLog(compute, maxLength, nullptr, infoLog);

		// We dont need the shader anymore.
		glDeleteShader(compute);

		// Log the error to the console
		FR_CORE_ASSERT(false, infoLog);

		delete[] infoLog;
	}

	m_RendererID = glCreateProgram();
	glAttachShader(m_RendererID, compute);
	glLinkProgram(m_RendererID);

	//Check it's compilation status
	glGetProgramiv(m_RendererID, GL_LINK_STATUS, &success);

	//Check if we succesfully compiled the shader
	if (!success)
	{
		// Get error infos
		int maxLength = 0;
		glGetProgramiv(m_RendererID, GL_INFO_LOG_LENGTH, &maxLength);

		// Max length includes the null character
		char* infoLog = new char[maxLength];
		glGetProgramInfoLog(m_RendererID, maxLength, nullptr, infoLog);

		// We dont need the shader anymore.
		glDeleteProgram(m_RendererID);
		glDeleteShader(compute);

		// Log the error to the console
		FR_CORE_ASSERT(false, infoLog);

		delete[] infoLog;
	}

}
