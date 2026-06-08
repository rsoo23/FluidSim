#pragma once

#include <glad/glad.h> // include glad to get all the required OpenGL headers
  
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <filesystem>

class Shader
{
public:
    // the program ID
    unsigned int programId;
  
    // constructor reads and builds the shader
    Shader(const std::filesystem::path& vertexPath, const std::filesystem::path& fragmentPath);

    // use / activate the shader
    void use();

    // utility uniform functions
    void setBool(const std::string &name, bool value) const;  
    void setInt(const std::string &name, int value) const;   
    void setFloat(const std::string &name, float value) const;

private:
    enum class ShaderType {
        VERTEX,
        FRAGMENT,
        PROGRAM
    };
    void checkShaderError(unsigned int id, ShaderType type) const;
};
  