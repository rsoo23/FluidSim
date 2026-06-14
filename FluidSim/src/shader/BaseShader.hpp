#pragma once

#include <filesystem>

class BaseShader
{
public:
    BaseShader(const std::filesystem::path& path, GLenum shaderType);
    virtual ~BaseShader();

	BaseShader()							    = delete;
	BaseShader(const BaseShader&)		        = delete;
	BaseShader& operator=(BaseShader&&)			= delete;
	BaseShader& operator=(const BaseShader&)    = delete;
	BaseShader(BaseShader&&)					= delete;

    void use() const;

    void setFloat(const std::string& name, float value) const;
    void setUint(const std::string& name, unsigned int value) const;
    void setVec2(const std::string& name, glm::vec2 v) const;

protected:
    GLuint m_ProgramId;
    std::filesystem::path m_Path;
    void checkShaderCompileError(GLuint id) const;
    void checkProgramLinkError(GLuint id) const;
};
