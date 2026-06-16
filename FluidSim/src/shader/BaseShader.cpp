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
	const auto fullPath = std::filesystem::current_path() / path;

	if (!std::filesystem::exists(fullPath))
	{
		throw std::runtime_error("Shader not found: " + path.string());
	}

	if (!std::filesystem::is_regular_file(fullPath))
	{
		throw std::runtime_error("Shader path is not a regular file: " + path.string());
	}

	file.exceptions(std::ios::failbit | std::ios::badbit);
	try
	{
		file.open(std::filesystem::current_path() / path);
	}
	catch (const std::ios_base::failure& e)
	{
		throw std::runtime_error("Failed to open shader: " + path.string() + "(" + e.code().message() + ")");
	}
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
		throw std::runtime_error("Failed to compile shader " + path.string() + ": " + infoLog);
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
		throw std::runtime_error("Failed to link shader program: " + infoLog);
	}
}

BaseShader::~BaseShader()
{
	glDeleteProgram(m_ProgramId);
}
