#include "pch.h"
#include "BaseShader.hpp"
#include "ComputeShader.hpp"

ComputeShader::ComputeShader(const std::filesystem::path& path, unsigned int screenWidth, unsigned int screenHeight) :
	BaseShader{},
	m_ScreenWidth{ screenWidth },
	m_ScreenHeight{ screenHeight },
	m_GroupSizeX{ static_cast<GLuint>(std::ceil(screenWidth / WORKGROUP_SIZE_X)) },
	m_GroupSizeY{ static_cast<GLuint>(std::ceil(screenHeight / WORKGROUP_SIZE_Y)) }
{
	GLuint shaderId{ createShader(path, GL_COMPUTE_SHADER) };

    // create shader program
	m_ProgramId = glCreateProgram();
	glAttachShader(m_ProgramId, shaderId);
	glLinkProgram(m_ProgramId);
	checkProgramLinkError(m_ProgramId);

	glDeleteShader(shaderId);
}

void ComputeShader::dispatch() const
{
	glDispatchCompute(m_GroupSizeX, m_GroupSizeY, 1);
	glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
}

void ComputeShader::dispatchFinal() const
{
	glDispatchCompute(m_GroupSizeX, m_GroupSizeY, 1);
	glMemoryBarrier(GL_TEXTURE_FETCH_BARRIER_BIT);
}

void ComputeShader::bindImageTexture(GLuint unit, GLuint tex, GLenum access, GLenum format)
{
	glBindImageTexture(unit, tex, 0, GL_FALSE, 0, access, format);
}
