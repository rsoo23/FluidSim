#pragma once

class ComputeShader
{
public:
    ComputeShader(const std::filesystem::path& path);

	ComputeShader()                 				= default;
	ComputeShader(const ComputeShader&)				= delete;
	ComputeShader& operator=(const ComputeShader&)  = delete;
	ComputeShader(ComputeShader&&)					= delete;
	ComputeShader& operator=(ComputeShader&&)		= default;

    void use();
    void useFinal();

    void bindImageTexture(GLuint unit, GLuint tex, GLenum access, GLenum format);

    void setFloat(const std::string& name, float value) const;
    void setVec2(const std::string& name, glm::vec2 v) const;

    GLuint getProgramId() const;

private:
    GLuint programId;
    enum class ShaderType {
        COMPUTE_SHADER,
        PROGRAM
    };
    void checkComputeShaderError(GLuint id, ShaderType type) const;
};
  