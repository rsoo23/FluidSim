#pragma once

class ComputeShader
{
public:
    ComputeShader(const std::filesystem::path& path, unsigned int screenWidth, unsigned int screenHeight);

	ComputeShader()                 				= default;
	ComputeShader& operator=(ComputeShader&&)		= default;
    ~ComputeShader()                                = default;

	ComputeShader(const ComputeShader&)				= delete;
	ComputeShader& operator=(const ComputeShader&)  = delete;
	ComputeShader(ComputeShader&&)					= delete;

    void use();
    void dispatch();
    void dispatchFinal();

    void bindImageTexture(GLuint unit, GLuint tex, GLenum access, GLenum format);

    void setFloat(const std::string& name, float value) const;
    void setVec2(const std::string& name, glm::vec2 v) const;

    GLuint getProgramId() const;

private:
	static constexpr unsigned int WORKGROUP_SIZE_X = 8;
	static constexpr unsigned int WORKGROUP_SIZE_Y = 8;

    GLuint programId;
    unsigned int m_ScreenWidth;
	unsigned int m_ScreenHeight;
    unsigned int m_GroupSizeX;
	unsigned int m_GroupSizeY;

    enum class ShaderType {
        COMPUTE_SHADER,
        PROGRAM
    };
    void checkComputeShaderError(GLuint id, ShaderType type) const;
};
  