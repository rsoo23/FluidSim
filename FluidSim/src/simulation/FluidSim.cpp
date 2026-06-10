#include "pch.h"
#include "FluidSim.hpp"

FluidSim::FluidSim(int screenWidth, int screenHeight):
	m_ScreenWidth(screenWidth),
	m_ScreenHeight(screenHeight),
	m_DiffuseFactor(1.f),
	m_Viscosity(1.f),
	m_DeltaTime(1 / 60)
{
	// compute shader textures setup
	m_VelTexture		= generateTexture(800, 600,  GL_RG32F, GL_RG, 1, GL_RG32F);
	m_VelTextureNext	= generateTexture(800, 600,  GL_RG32F, GL_RG, 2, GL_RG32F);

	m_PresTexture		= generateTexture(800, 600,  GL_R32F, GL_RED, 3, GL_R32F);
	m_PresTextureNext	= generateTexture(800, 600,  GL_R32F, GL_RED, 4, GL_R32F);

	m_DivTexture		= generateTexture(800, 600,  GL_R32F, GL_RED, 5, GL_R32F);

	m_DensTexture		= generateTexture(800, 600,  GL_R32F, GL_RED, 6, GL_R32F);
	m_DensTextureNext	= generateTexture(800, 600,  GL_R32F, GL_RED, 7, GL_R32F);
}

void FluidSim::step()
{
	// add forces

	// diffuse velocities
	// project

	// advect velocities
	// project

	// diffuse densities
	// advect densities

	// render to texture? return texture values?
}

GLuint FluidSim::generateTexture(int w, int h, GLint internalFormat, GLenum texImage2DFormat, GLuint unit, GLenum bindImageTexFormat)
{
	GLuint texId = 0;
	glGenTextures(1, &texId);
	glBindTexture(GL_TEXTURE_2D, texId);
	glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, 800, 600, 0, texImage2DFormat, GL_FLOAT, NULL);
	glBindImageTexture(unit, texId, 0, GL_FALSE, 0, GL_READ_WRITE, bindImageTexFormat);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	return texId;
}

FluidSim::~FluidSim() {}
