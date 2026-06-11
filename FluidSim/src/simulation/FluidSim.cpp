#include "pch.h"
#include "FluidSim.hpp"
#include "shader/ComputeShader.hpp"

FluidSim::FluidSim(int screenWidth, int screenHeight):
	m_ScreenWidth(screenWidth),
	m_ScreenHeight(screenHeight),
	m_DiffuseFactor(1.f),
	m_Viscosity(1.f),
	m_DeltaTime(1 / 60),
	m_JacobiIterations(20),
	m_A(m_DiffuseFactor * m_DeltaTime),
	m_C(1.f * 4.f * m_A)
{
	// compute shader textures setup
	m_VelXTexture		= generateTexture(800, 600, GL_R32F);
	m_VelXTextureNext	= generateTexture(800, 600, GL_R32F);
	m_VelYTexture		= generateTexture(800, 600, GL_R32F);
	m_VelYTextureNext	= generateTexture(800, 600, GL_R32F);

	m_PresTexture		= generateTexture(800, 600, GL_R32F);
	m_PresTextureNext	= generateTexture(800, 600, GL_R32F);

	m_DivTexture		= generateTexture(800, 600, GL_R32F);

	m_DensTexture		= generateTexture(800, 600, GL_R32F);
	m_DensTextureNext	= generateTexture(800, 600, GL_R32F);

	// fill textures with 0
	glClearTexImage(m_VelXTexture, 0, GL_RED, GL_FLOAT, NULL);
	glClearTexImage(m_VelXTextureNext, 0, GL_RED, GL_FLOAT, NULL);
	glClearTexImage(m_VelYTexture, 0, GL_RED, GL_FLOAT, NULL);
	glClearTexImage(m_VelYTextureNext, 0, GL_RED, GL_FLOAT, NULL);
	glClearTexImage(m_PresTexture, 0, GL_RED, GL_FLOAT, NULL);
	glClearTexImage(m_PresTextureNext, 0, GL_RED, GL_FLOAT, NULL);
	glClearTexImage(m_DivTexture, 0, GL_RED, GL_FLOAT, NULL);
	glClearTexImage(m_DensTexture, 0, GL_RED, GL_FLOAT, NULL);
	glClearTexImage(m_DensTextureNext, 0, GL_RED, GL_FLOAT, NULL);

	// compute shader setup
	m_AddForceShader	= ComputeShader{ R"(shaders\addForce.comp)" };
	m_AdvectShader		= ComputeShader{ R"(shaders\advect.comp)" };
	m_DiffuseShader		= ComputeShader{ R"(shaders\diffuse.comp)" };
	m_JacobiShader		= ComputeShader{ R"(shaders\jacobi.comp)" };
	m_ProjectShader		= ComputeShader{ R"(shaders\project.comp)" };
}

void FluidSim::step(glm::vec2 mousePos)
{
	// add forces
	//m_AddForceShader.setFloat("deltaTime", m_DeltaTime);
	m_AddForceShader.bindImageTexture(0, m_VelXTexture, GL_READ_WRITE, GL_R32F);
	m_AddForceShader.bindImageTexture(1, m_VelYTexture, GL_READ_WRITE, GL_R32F);
	m_AddForceShader.bindImageTexture(2, m_DensTexture, GL_READ_WRITE, GL_R32F);
	m_AddForceShader.use();
	m_AddForceShader.setVec2("mousePos", mousePos);
	m_AddForceShader.setVec2("mouseForce", glm::vec2(2, 2));
	m_AddForceShader.setFloat("newDens", 0.25f); // density that will be added
	m_AddForceShader.setFloat("radius", 10.f); // radius of influence

	// diffuse velocities
	for (int i = 0; i < m_JacobiIterations; ++i)
	{
		m_JacobiShader.bindImageTexture(0, m_VelXTexture, GL_READ_ONLY, GL_R32F);
		m_JacobiShader.bindImageTexture(1, m_VelXTexture, GL_READ_ONLY, GL_R32F);
		m_JacobiShader.bindImageTexture(2, m_VelXTextureNext, GL_WRITE_ONLY, GL_R32F);
		m_JacobiShader.use();
		m_JacobiShader.setFloat("a", m_A);
		m_JacobiShader.setFloat("c", m_C);
		m_JacobiShader.setFloat("screenWidth", m_ScreenWidth);
		m_JacobiShader.setFloat("screenHeight", m_ScreenHeight);
		std::swap(m_VelXTexture, m_VelXTextureNext);
	}
	for (int i = 0; i < m_JacobiIterations; ++i)
	{
		m_JacobiShader.bindImageTexture(0, m_VelYTexture, GL_READ_ONLY, GL_R32F);
		m_JacobiShader.bindImageTexture(1, m_VelYTexture, GL_READ_ONLY, GL_R32F);
		m_JacobiShader.bindImageTexture(2, m_VelYTextureNext, GL_WRITE_ONLY, GL_R32F);
		m_JacobiShader.use();
		m_JacobiShader.setFloat("a", m_A);
		m_JacobiShader.setFloat("c", m_C);
		m_JacobiShader.setFloat("screenWidth", m_ScreenWidth);
		m_JacobiShader.setFloat("screenHeight", m_ScreenHeight);
		std::swap(m_VelYTexture, m_VelYTextureNext);
	}

	// project

	// advect velocities
	// project

	// diffuse densities
	// advect densities

	// render to texture? return texture values?
}

GLuint FluidSim::generateTexture(int w, int h, GLenum internalFormat)
{
	GLuint texId;
	glCreateTextures(GL_TEXTURE_2D, 1, &texId);
	glTextureStorage2D(texId, 1, internalFormat, w, h);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	return texId;
}

GLuint FluidSim::getFinalTexture() const
{
	return m_DensTexture;
}

FluidSim::~FluidSim() {}
