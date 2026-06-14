#pragma once

#include "BaseShader.hpp"

class ComputeShader: public BaseShader
{
public:
    ComputeShader(const std::filesystem::path& path, unsigned int screenWidth, unsigned int screenHeight);

    ~ComputeShader()                                = default;

	ComputeShader()                 				= delete;
	ComputeShader& operator=(ComputeShader&&)		= delete;
	ComputeShader(const ComputeShader&)				= delete;
	ComputeShader& operator=(const ComputeShader&)  = delete;
	ComputeShader(ComputeShader&&)					= delete;

    void dispatch() const;
    void dispatchFinal() const;

    void bindImageTexture(GLuint unit, GLuint tex, GLenum access, GLenum format);

private:
    static constexpr unsigned int WORKGROUP_SIZE_X{ 8u };
    static constexpr unsigned int WORKGROUP_SIZE_Y{ 8u };
    GLuint m_GroupSizeX;
	GLuint m_GroupSizeY;
    unsigned int m_ScreenWidth;
	unsigned int m_ScreenHeight;
};
