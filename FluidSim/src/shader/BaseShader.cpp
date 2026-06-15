#include "pch.h"
#include "BaseShader.hpp"

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

GLuint BaseShader::createShader(const std::filesystem::path& path, GLenum shaderType)
{
	std::string code{};
	std::ifstream file{};
	std::stringstream shaderStream{};
	GLuint shaderId{ glCreateShader(shaderType) };

	// extract shader source code
	file.open(std::filesystem::current_path() / path);
	shaderStream << file.rdbuf();
	file.close();
	code = shaderStream.str();

	// create shader
	const char* codeCStr{ code.c_str() };

	glShaderSource(shaderId, 1, &codeCStr, NULL);
	glCompileShader(shaderId);
	checkShaderCompileError(shaderId, path);

	return shaderId;
}

void BaseShader::checkShaderCompileError(GLuint shaderId, const std::filesystem::path& path) const
{
	GLint success{};
	GLint logLength{};

	glGetShaderiv(shaderId, GL_COMPILE_STATUS, &success);
	if(!success)
	{
		glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &logLength);
		std::string infoLog(logLength, '\0');

		glGetShaderInfoLog(shaderId, 512, NULL, infoLog.data());
		std::cerr << "ERROR::SHADER::COMPILATION_FAILED (" << path <<  ")\n" << infoLog.data() << "\n";
	};
}

void BaseShader::checkProgramLinkError(GLuint programId) const
{
	GLint success{};
	GLint logLength{};

	glGetProgramiv(programId, GL_LINK_STATUS, &success);
	if(!success)
	{
		glGetShaderiv(programId, GL_INFO_LOG_LENGTH, &logLength);
		std::string infoLog(logLength, '\0');

		glGetProgramInfoLog(programId, 512, NULL, infoLog.data());
		std::cerr << "ERROR::PROGRAM::LINKING_FAILED\n" << infoLog.data() << "\n";
	}
}

BaseShader::~BaseShader()
{
	glDeleteProgram(m_ProgramId);
}
