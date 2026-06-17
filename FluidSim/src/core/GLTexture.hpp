#pragma once

#include <glad/glad.h>

class GLTexture {
public:
	GLTexture(GLenum internalFormat, GLenum format, unsigned int screenWidth, unsigned int screenHeight);
	GLTexture(GLTexture&& other) noexcept;
	GLTexture& operator=(GLTexture&& other) noexcept;
	~GLTexture();

	GLTexture(const GLTexture&)				= delete;
	GLTexture& operator=(const GLTexture&)	= delete;

	GLuint getTex() const;

private:
	GLuint m_Tex{ 0 };
	void generateTexture(GLenum internalFormat, unsigned int screenWidth, unsigned int screenHeight);
	void setEmptyTexture(GLenum format) const;
	void release();
};