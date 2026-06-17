#include "pch.h"
#include "BaseShader.hpp"
#include "VertFragShader.hpp"

VertFragShader::VertFragShader(const std::filesystem::path& vertPath, const std::filesystem::path& fragPath) : BaseShader{}
{
	GLuint vertShaderId{ createShader(vertPath, GL_VERTEX_SHADER) };
	GLuint fragShaderId{ createShader(fragPath, GL_FRAGMENT_SHADER) }; 

	// create shader program
	m_ProgramId = glCreateProgram();
	glAttachShader(m_ProgramId, vertShaderId);
	glAttachShader(m_ProgramId, fragShaderId);
	glLinkProgram(m_ProgramId);
	checkProgramLinkError(m_ProgramId);

	glDeleteShader(vertShaderId);
	glDeleteShader(fragShaderId);
}

