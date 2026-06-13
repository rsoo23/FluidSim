#include "pch.h"
#include "ComputeShader.hpp"

ComputeShader::ComputeShader(const std::filesystem::path& path, unsigned int screenWidth, unsigned int screenHeight) :
	m_ScreenWidth(screenWidth),
	m_ScreenHeight(screenHeight),
	m_GroupSizeX(std::ceil(screenWidth / WORKGROUP_SIZE_X)),
	m_GroupSizeY(std::ceil(screenHeight/ WORKGROUP_SIZE_Y))
{
	// retrieve the compute shader source code from path
    std::string code;
    std::ifstream file;
    // ensure ifstream objects can throw exceptions:
    file.exceptions (std::ifstream::failbit | std::ifstream::badbit);
    try 
    {
        // open files
        file.open(path);
        std::stringstream shaderStream;
        // read file's buffer contents into stream
        shaderStream << file.rdbuf();
        // close file handler
        file.close();
        // convert stream into string
        code = shaderStream.str();
    }
    catch(std::ifstream::failure e)
    {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
    }
    const char* shaderSource = code.c_str();

	GLuint computeShader = glCreateShader(GL_COMPUTE_SHADER);
	glShaderSource(computeShader, 1, &shaderSource, NULL);
	glCompileShader(computeShader);
	checkComputeShaderError(computeShader, ShaderType::COMPUTE_SHADER);

    // shader program
	programId = glCreateProgram();
	glAttachShader(programId, computeShader);
	glLinkProgram(programId);
	checkComputeShaderError(programId, ShaderType::PROGRAM);
	  
	glDeleteShader(computeShader);
}

void ComputeShader::use()
{
	glUseProgram(programId);
}

void ComputeShader::dispatch()
{
	glDispatchCompute(m_GroupSizeX, m_GroupSizeY, 1);
	glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
}

void ComputeShader::dispatchFinal()
{
	glDispatchCompute(m_GroupSizeX, m_GroupSizeY, 1);
	glMemoryBarrier(GL_TEXTURE_FETCH_BARRIER_BIT);
}

void ComputeShader::bindImageTexture(GLuint unit, GLuint tex, GLenum access, GLenum format)
{
	glBindImageTexture(unit, tex, 0, GL_FALSE, 0, access, format);
}

void ComputeShader::setFloat(const std::string &name, float value) const
{ 
    glUniform1f(glGetUniformLocation(programId, name.c_str()), value); 
} 

void ComputeShader::setVec2(const std::string &name, glm::vec2 v) const
{ 
    glUniform2f(glGetUniformLocation(programId, name.c_str()), v.x, v.y); 
} 

GLuint ComputeShader::getProgramId() const
{ 
	return programId;
} 

void ComputeShader::checkComputeShaderError(GLuint shaderId, ShaderType type) const
{
	int success;
	char infoLog[512];

	switch (type) {
		case ShaderType::COMPUTE_SHADER:
			glGetShaderiv(shaderId, GL_COMPILE_STATUS, &success);
			if(!success)
			{
				glGetShaderInfoLog(shaderId, 512, NULL, infoLog);
				std::cout << "ERROR::COMPUTE_SHADER::COMPILATION_FAILED\n" << infoLog << std::endl;
			};
			break;
		case ShaderType::PROGRAM:
			glGetProgramiv(shaderId, GL_LINK_STATUS, &success);
			if(!success)
			{
				glGetProgramInfoLog(shaderId, 512, NULL, infoLog);
				std::cout << "ERROR::COMPUTE_SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
			}
			break;
		default:
			std::cout << "ERROR::COMPUTE_SHADER::INVALID_TYPE\n" << std::endl;
			break;
	}
}
