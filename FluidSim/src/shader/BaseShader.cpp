#include "pch.h"
#include "BaseShader.hpp"

BaseShader::BaseShader(const std::filesystem::path& path, GLenum shaderType) : m_Path{ path }
{
	std::string code{};
	std::ifstream file{};
	std::stringstream shaderStream{};

	// extract shader source code
	file.open(std::filesystem::current_path() / path);
	shaderStream << file.rdbuf();
	file.close();
	code = shaderStream.str();

	// create shader
	const char* cStrCode = code.c_str();
	GLuint shaderId{ glCreateShader(shaderType) };

	glShaderSource(shaderId, 1, &cStrCode, NULL);
	glCompileShader(shaderId);
	checkShaderCompileError(shaderId);

    // create shader program
	m_ProgramId = glCreateProgram();
	glAttachShader(m_ProgramId, shaderId);
	glLinkProgram(m_ProgramId);
	checkProgramLinkError(m_ProgramId);

	glDeleteShader(shaderId);
}

void BaseShader::use() const
{
	glUseProgram(m_ProgramId);
}

void BaseShader::setFloat(const std::string &name, float value) const
{ 
    glUniform1f(glGetUniformLocation(m_ProgramId, name.c_str()), value); 
} 

void BaseShader::setUint(const std::string &name, unsigned int value) const
{ 
    glUniform1ui(glGetUniformLocation(m_ProgramId, name.c_str()), value); 
} 

void BaseShader::setVec2(const std::string &name, glm::vec2 v) const
{ 
    glUniform2f(glGetUniformLocation(m_ProgramId, name.c_str()), v.x, v.y); 
} 

void BaseShader::checkShaderCompileError(GLuint shaderId) const
{
	GLint success{};
	GLint logLength{};

	glGetShaderiv(shaderId, GL_COMPILE_STATUS, &success);
	if(!success)
	{
		glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &logLength);
		std::string infoLog(logLength, '\0');

		glGetShaderInfoLog(shaderId, 512, NULL, infoLog.data());
		std::cerr << "ERROR::SHADER::COMPILATION_FAILED (" << m_Path <<  ")\n" << infoLog.data() << "\n";
	};
}

void BaseShader::checkProgramLinkError(GLuint shaderId) const
{
	GLint success{};
	GLint logLength{};

	glGetProgramiv(shaderId, GL_LINK_STATUS, &success);
	if(!success)
	{
		glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &logLength);
		std::string infoLog(logLength, '\0');

		glGetProgramInfoLog(shaderId, 512, NULL, infoLog.data());
		std::cerr << "ERROR::PROGRAM::LINKING_FAILED (" << m_Path <<  ")\n" << infoLog.data() << "\n";
	}
}

BaseShader::~BaseShader()
{
	glDeleteProgram(m_ProgramId);
}
