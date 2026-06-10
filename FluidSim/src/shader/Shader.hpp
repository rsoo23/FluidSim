#pragma once

#include <glad/glad.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <filesystem>

class Shader
{
public:
    Shader(const std::filesystem::path& vertexPath, const std::filesystem::path& fragmentPath);

	Shader()                 			= delete;
	Shader(const Shader&)				= delete;
	Shader& operator=(const Shader&)	= delete;
	Shader(Shader&&)					= delete;
	Shader& operator=(Shader&&)		    = delete;

    void use();

private:
    unsigned int programId;
    enum class ShaderType {
        VERTEX,
        FRAGMENT,
        PROGRAM
    };
    void checkShaderError(unsigned int id, ShaderType type) const;
};
  