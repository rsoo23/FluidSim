#pragma once

#include <filesystem>

class BaseShader
{
public:
    BaseShader() = default;
    virtual ~BaseShader();

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
    GLuint createShader(const std::filesystem::path& path, GLenum shaderType);
    void checkProgramLinkError(GLuint id) const;

private:
    void checkShaderCompileError(GLuint id, const std::filesystem::path& path) const;
};
