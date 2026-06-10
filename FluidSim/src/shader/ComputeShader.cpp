#include "pch.h"
#include "ComputeShader.hpp"

ComputeShader::ComputeShader(const std::filesystem::path& path)
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
	glDispatchCompute(std::ceil(800 / 8), std::ceil(600 / 8), 1);
	glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
}

void setValues(std::vector<float>& values)
{
	glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F, 800, 600, 0, GL_RED, GL_FLOAT, values.data());
}

std::vector<float> get_values() {
    unsigned int collection_size = 800 * 600;
    std::vector<float> compute_data(collection_size);
    glGetTexImage(GL_TEXTURE_2D, 0, GL_RED, GL_FLOAT, compute_data.data());
    return compute_data;
}

void ComputeShader::checkComputeShaderError(unsigned int shaderId, ShaderType type) const
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
