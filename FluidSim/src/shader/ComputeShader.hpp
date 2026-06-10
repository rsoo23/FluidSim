#pragma once

#include <glad/glad.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <filesystem>

class ComputeShader
{
public:
    ComputeShader(const std::filesystem::path& path);

	ComputeShader()                 				= delete;
	ComputeShader(const ComputeShader&)				= delete;
	ComputeShader& operator=(const ComputeShader&)	= delete;
	ComputeShader(ComputeShader&&)					= delete;
	ComputeShader& operator=(ComputeShader&&)		= delete;

    void use();

private:
    unsigned int programId;
    enum class ShaderType {
        COMPUTE_SHADER,
        PROGRAM
    };
    void checkComputeShaderError(unsigned int id, ShaderType type) const;
};
  