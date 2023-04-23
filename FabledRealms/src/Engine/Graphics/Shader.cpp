#include "frpch.h"
#include <fstream>
#include <sstream>
#include <filesystem>

#include "Shader.h"

#include <glad/glad.h>

Shader::Shader(const char* vertexPath, const char* fragmentPath)
{
	//Ref from: https://learnopengl.com/Getting-started/Shaders
	// and  https://www.khronos.org/opengl/wiki/Shader_Compilation

	std::filesystem::path cwd = std::filesystem::current_path();
	DLOG_CORE_INFO(cwd.string());

	std::string vertexCode;
	std::string fragmentCode;

	std::ifstream vShaderFile;
	std::ifstream fShaderFile;


	vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	// ------- Vertex Shader ------------
	try
	{
		vShaderFile.open(vertexPath);

		std::stringstream vShaderStream;

		vShaderStream << vShaderFile.rdbuf();
		vertexCode = vShaderStream.str();

	}
	catch (std::ifstream::failure e)
	{
		LOG_CORE_ERROR("Failed To Read Vertex Shader at: " << vertexPath);
	}

	// ------- Fragment Shader ------------
	try
	{
		fShaderFile.open(fragmentPath);

		std::stringstream fShaderStream;

		fShaderStream << fShaderFile.rdbuf();
		fragmentCode = fShaderStream.str();

	}
	catch (std::ifstream::failure e)
	{
		LOG_CORE_ERROR("Failed To Read Fragment Shader at: " << fragmentPath);
	}

	// Compile Shaders
	
	// Vertex
	int success = 0;
	const char* src = vertexCode.c_str();
	
	int32_t vShaderID = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vShaderID, 1, &src, nullptr);

	glCompileShader(vShaderID);
	glGetShaderiv(vShaderID, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		//Get error infos
		int maxLength = 0;
		glGetShaderiv(vShaderID, GL_INFO_LOG_LENGTH, &maxLength);

		//Max length includes the null character
		char* infoLog = new char[maxLength];
		glGetShaderInfoLog(vShaderID, maxLength, nullptr, infoLog);

		//We dont need the shader anymore.
		glDeleteShader(vShaderID);

		LOG_CORE_ERROR(infoLog);

		delete[] infoLog;
	}

	// ---------- Fragment Shader ----------------------------

	src = fragmentCode.c_str();
	int fShaderID = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fShaderID, 1, &src, nullptr);

	glCompileShader(fShaderID);
	glGetShaderiv(fShaderID, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		//Get error infos
		int maxLength = 0;
		glGetShaderiv(fShaderID, GL_INFO_LOG_LENGTH, &maxLength);

		//Max length includes the null character
		char* infoLog = new char[maxLength];
		glGetShaderInfoLog(fShaderID, maxLength, nullptr, infoLog);

		//We dont need the shader anymore.
		glDeleteShader(vShaderID);
		glDeleteShader(fShaderID);

		LOG_CORE_ERROR(infoLog);
		
		delete[] infoLog;
	}

	// Create the Shader Program

	m_RendererID = glCreateProgram();
	glAttachShader(m_RendererID, vShaderID);
	glAttachShader(m_RendererID, fShaderID);

	//Link the program
	glLinkProgram(m_RendererID);
	
	glGetProgramiv(m_RendererID, GL_LINK_STATUS, &success);
	if (!success)
	{
		int maxLength = 0;
		glGetProgramiv(m_RendererID, GL_INFO_LOG_LENGTH, &maxLength);

		//Max length includes the null character
		char* infoLog = new char[maxLength];
		glGetProgramInfoLog(m_RendererID, maxLength, nullptr, infoLog);

		//Compilation failed, delete everythng
		glDeleteProgram(m_RendererID);

		glDeleteShader(vShaderID);
		glDeleteShader(fShaderID);

		LOG_CORE_ERROR(infoLog);

		delete[] infoLog;
	}

	// Always detach shaders after a successful link.
	glDetachShader(m_RendererID, vShaderID);
	glDetachShader(m_RendererID, fShaderID);
}

Shader::~Shader()
{
	glDeleteProgram(m_RendererID);
}

void Shader::Use() const
{
	glUseProgram(m_RendererID);
}

void Shader::SetFloat(const char* name, float value)
{
	glUniform1f(glGetUniformLocation(m_RendererID, name), value);
}


