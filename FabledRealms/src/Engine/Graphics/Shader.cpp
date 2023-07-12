#include "frpch.h"
#include <fstream>
#include <sstream>
#include "Shader.h"

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

Shader::Shader(const char* vertexPath, const char* fragmentPath, const char* geometryPath)
{
	// Ref from: https://learnopengl.com/Getting-started/Shaders
	// and  https://www.khronos.org/opengl/wiki/Shader_Compilation

	
	std::string vertexCode;
	std::string fragmentCode;
	std::string geometryCode;

	std::ifstream vShaderFile;
	std::ifstream fShaderFile;
	std::ifstream gShaderFile;


	// Make sure we can throw exceptions
	vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	gShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);


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

	if (geometryPath != nullptr)
	{
		try
		{
			gShaderFile.open(geometryPath);
			std::stringstream gShaderStream;
			gShaderStream << gShaderFile.rdbuf();
			gShaderFile.close();
			geometryCode = gShaderStream.str();
		}
		catch (std::ifstream::failure& e)
		{
			FR_CORE_ASSERT(false, "Geometry Shader file not succesfully read: " << e.what());
		}
	}


	// Compile Shaders
	
	// Vertex
	int success = 0;

	const char* src = vertexCode.c_str();
	
	//Create a shader in the GPU
	int32_t vShaderID = glCreateShader(GL_VERTEX_SHADER);

	//Set the sourcecode of the shader
	glShaderSource(vShaderID, 1, &src, nullptr);

	//Compile the shader
	glCompileShader(vShaderID);

	//Check it's compilation status
	glGetShaderiv(vShaderID, GL_COMPILE_STATUS, &success);

	//Check if we succesfully compiled the shader
	if (!success)
	{
		// Get error infos
		int maxLength = 0;
		glGetShaderiv(vShaderID, GL_INFO_LOG_LENGTH, &maxLength);

		// Max length includes the null character
		char* infoLog = new char[maxLength];
		glGetShaderInfoLog(vShaderID, maxLength, nullptr, infoLog);

		// We dont need the shader anymore.
		glDeleteShader(vShaderID);
		
		// Log the error to the console
		FR_CORE_ASSERT(false, infoLog);

		delete[] infoLog;
	}

	// ---------- Fragment Shader ----------------------------

	src = fragmentCode.c_str();

	//Create a fragment shader in the GPU
	int fShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	//Set the sourcecode of the shader
	glShaderSource(fShaderID, 1, &src, nullptr);

	//Compile the shader
	glCompileShader(fShaderID);

	//Check it's status
	glGetShaderiv(fShaderID, GL_COMPILE_STATUS, &success);

	//Check if it was a success
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

		FR_CORE_ASSERT(false, infoLog);
		
		delete[] infoLog;
	}


	// -------------- Geometry -----------------------


	int gShaderID;
	if (geometryPath != nullptr)
	{
		const char* gShaderCode = geometryCode.c_str();
		gShaderID = glCreateShader(GL_GEOMETRY_SHADER);
		glShaderSource(gShaderID, 1, &gShaderCode, NULL);
		glCompileShader(gShaderID);
		
		//Check it's status
		glGetShaderiv(gShaderID, GL_COMPILE_STATUS, &success);

		//Check if it was a success
		if (!success)
		{
			//Get error infos
			int maxLength = 0;
			glGetShaderiv(gShaderID, GL_INFO_LOG_LENGTH, &maxLength);

			//Max length includes the null character
			char* infoLog = new char[maxLength];
			glGetShaderInfoLog(gShaderID, maxLength, nullptr, infoLog);

			//We dont need the shader anymore.
			glDeleteShader(vShaderID);
			glDeleteShader(fShaderID);

			FR_CORE_ASSERT(false, infoLog);

			delete[] infoLog;
		}
	}






	// Create the Shader Program
	m_RendererID = glCreateProgram();

	//Attached the compiled shaders
	glAttachShader(m_RendererID, vShaderID);
	glAttachShader(m_RendererID, fShaderID);

	if (geometryPath != nullptr)
	{
		glAttachShader(m_RendererID, gShaderID); 
	}

	//Link the program
	glLinkProgram(m_RendererID);
	
	//Check the linking status
	glGetProgramiv(m_RendererID, GL_LINK_STATUS, &success);

	if (!success)
	{
		//Get the length of the error info
		int maxLength = 0;
		glGetProgramiv(m_RendererID, GL_INFO_LOG_LENGTH, &maxLength);

		//Note: Max length includes the null character
		char* infoLog = new char[maxLength];

		//Get the error info
		glGetProgramInfoLog(m_RendererID, maxLength, nullptr, infoLog);

		//Compilation failed, delete everythng
		glDeleteProgram(m_RendererID);
		glDeleteShader(vShaderID);
		glDeleteShader(fShaderID);

		//Log the error
		FR_CORE_ASSERT(false, infoLog);

		delete[] infoLog;
	}

	// Always detach shaders after a successful link.
	glDetachShader(m_RendererID, vShaderID);
	glDetachShader(m_RendererID, fShaderID);
	if (geometryPath != nullptr)
		glDeleteShader(gShaderID);
}

Shader::~Shader()
{
	glDeleteProgram(m_RendererID);
}

void Shader::Use() const
{
	glUseProgram(m_RendererID);
}

void Shader::SetInt(const const char* name, int value)
{
	glUniform1i(glGetUniformLocation(m_RendererID, name), value);
}

void Shader::SetFloat(const char* name, float value)
{
	glUniform1f(glGetUniformLocation(m_RendererID, name), value);
}

void Shader::SetVec2(const char* name, glm::vec2 value)
{
	glUniform2f(glGetUniformLocation(m_RendererID, name), value.x, value.y);
}

void Shader::SetVec3(const char* name, glm::vec3 value)
{
	glUniform3f(glGetUniformLocation(m_RendererID, name), value.x, value.y, value.z);
}

void Shader::SetMat3(const char* name, const glm::mat3& value)
{
	glUniformMatrix3fv(glGetUniformLocation(m_RendererID, name), 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::SetMat4(const char* name, const glm::mat4& value)
{
	glUniformMatrix4fv(glGetUniformLocation(m_RendererID, name), 1, GL_FALSE, glm::value_ptr(value));
}


