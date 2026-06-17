#include "pch.h"
#include "GLTexture.hpp"

GLTexture::GLTexture(GLenum internalFormat, GLenum format, unsigned int screenWidth, unsigned int screenHeight)
{
	generateTexture(internalFormat, screenWidth, screenHeight);
	setEmptyTexture(format);
}

GLTexture::GLTexture(GLTexture&& other) noexcept : m_Tex{ other.m_Tex }
{
	other.m_Tex = 0;
}

GLTexture& GLTexture::operator=(GLTexture&& other) noexcept
{
	if (this != &other)
	{
		release();
		std::swap(m_Tex, other.m_Tex);
	}
	return *this;
}

GLuint GLTexture::getTex() const
{
	return m_Tex;
}

void GLTexture::generateTexture(GLenum internalFormat, unsigned int screenWidth, unsigned int screenHeight)
{
	glCreateTextures(GL_TEXTURE_2D, 1, &m_Tex);
	glTextureStorage2D(m_Tex, 1, internalFormat, static_cast<GLsizei>(screenWidth), static_cast<GLsizei>(screenHeight));

	glTextureParameteri(m_Tex, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTextureParameteri(m_Tex, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTextureParameteri(m_Tex, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTextureParameteri(m_Tex, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void GLTexture::setEmptyTexture(GLenum format) const
{
	glClearTexImage(m_Tex, 0, format, GL_FLOAT, NULL);
}

void GLTexture::release()
{
	glDeleteTextures(1, &m_Tex);
	m_Tex = 0;
}

GLTexture::~GLTexture()
{
	release();
}