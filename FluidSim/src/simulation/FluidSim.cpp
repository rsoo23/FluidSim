#include "pch.h"
#include "FluidSim.hpp"
#include "shader/ComputeShader.hpp"

FluidSim::FluidSim(int screenWidth, int screenHeight):
	m_ScreenWidth(static_cast<float>(screenWidth)),
	m_ScreenHeight(static_cast<float>(screenHeight)),
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
	m_JacobiShader		= ComputeShader{ R"(shaders\jacobi.comp)" };
	m_ProjectShader		= ComputeShader{ R"(shaders\project.comp)" };
	m_DivergenceShader	= ComputeShader{ R"(shaders\divergence.comp)" };
}

void FluidSim::step(glm::vec2 mousePos)
{
	// add forces
	addForce(mousePos, glm::vec2(2, 2), 0.25f, 10.f);

	// velocities:
	// diffuse
	diffuse(m_VelXTexture, m_VelXTextureNext);
	diffuse(m_VelYTexture, m_VelYTextureNext);
	// project
	project();
	// advect velocities
	advect(m_VelXTexture, m_VelXTextureNext, false);
	advect(m_VelYTexture, m_VelYTextureNext, false);
	// project
	project();

	// densities:
	// diffuse
	diffuse(m_DensTexture, m_DensTextureNext);
	// advect
	advect(m_DensTexture, m_DensTextureNext, true);
}

void FluidSim::addForce(glm::vec2 mousePos, glm::vec2 mouseForce, float newDens, float radius)
{
	m_AddForceShader.bindImageTexture(0, m_VelXTexture, GL_READ_WRITE, GL_R32F);
	m_AddForceShader.bindImageTexture(1, m_VelYTexture, GL_READ_WRITE, GL_R32F);
	m_AddForceShader.bindImageTexture(2, m_DensTexture, GL_READ_WRITE, GL_R32F);
	m_AddForceShader.use();
	m_AddForceShader.setVec2("mousePos", mousePos);
	m_AddForceShader.setVec2("mouseForce", mouseForce);
	m_AddForceShader.setFloat("newDens", newDens); // density that will be added
	m_AddForceShader.setFloat("radius", radius); // radius of influence
	m_AddForceShader.dispatch();
}

void FluidSim::diffuse(GLuint readTex, GLuint writeTex)
{
	jacobiSolve(readTex, readTex, writeTex, m_C);
}

void FluidSim::project()
{
	// divergence
	m_DivergenceShader.bindImageTexture(0, m_VelXTexture, GL_READ_ONLY, GL_R32F);
	m_DivergenceShader.bindImageTexture(1, m_VelYTexture, GL_READ_ONLY, GL_R32F);
	m_DivergenceShader.bindImageTexture(2, m_DivTexture, GL_WRITE_ONLY, GL_R32F);
	m_DivergenceShader.use();
	m_DivergenceShader.setFloat("screenWidth", m_ScreenWidth);
	m_DivergenceShader.setFloat("screenHeight", m_ScreenHeight);
	m_DivergenceShader.dispatch();
	// solve pressure poisson
	jacobiSolve(m_DivTexture, m_PresTexture, m_PresTextureNext, 4.f);
	// subtract pressure gradient for incompressibility
	m_ProjectShader.bindImageTexture(0, m_VelXTexture, GL_READ_WRITE, GL_R32F);
	m_ProjectShader.bindImageTexture(1, m_VelYTexture, GL_READ_WRITE, GL_R32F);
	m_ProjectShader.bindImageTexture(2, m_PresTexture, GL_READ_ONLY, GL_R32F);
	m_ProjectShader.use();
	m_ProjectShader.setFloat("screenWidth", m_ScreenWidth);
	m_ProjectShader.setFloat("screenHeight", m_ScreenHeight);
	m_ProjectShader.dispatch();
}

void FluidSim::advect(GLuint readTex, GLuint writeTex, bool isFinalStep)
{
	m_AdvectShader.bindImageTexture(0, readTex, GL_READ_ONLY, GL_R32F);
	m_AdvectShader.bindImageTexture(1, writeTex, GL_WRITE_ONLY, GL_R32F);
	m_AdvectShader.bindImageTexture(2, m_VelXTexture, GL_READ_ONLY, GL_R32F);
	m_AdvectShader.bindImageTexture(3, m_VelYTexture, GL_READ_ONLY, GL_R32F);
	m_AdvectShader.use();
	m_AdvectShader.setFloat("screenWidth", m_ScreenWidth);
	m_AdvectShader.setFloat("screenHeight", m_ScreenHeight);
	m_AdvectShader.setFloat("deltaTime", m_DeltaTime);
	if (isFinalStep)
	{
		m_AdvectShader.dispatchFinal();
	}
	else
{
		m_AdvectShader.dispatch();
	}
	std::swap(readTex, writeTex);
}

void FluidSim::jacobiSolve(GLuint readTex1, GLuint readTex2, GLuint writeTex, float c)
{
	for (int i = 0; i < m_JacobiIterations; ++i)
	{
		m_JacobiShader.bindImageTexture(0, readTex1, GL_READ_ONLY, GL_R32F);
		m_JacobiShader.bindImageTexture(1, readTex2, GL_READ_ONLY, GL_R32F);
		m_JacobiShader.bindImageTexture(2, writeTex, GL_WRITE_ONLY, GL_R32F);
		m_JacobiShader.use();
		m_JacobiShader.setFloat("a", m_A);
		m_JacobiShader.setFloat("c", c);
		m_JacobiShader.setFloat("screenWidth", m_ScreenWidth);
		m_JacobiShader.setFloat("screenHeight", m_ScreenHeight);
		m_JacobiShader.dispatch();
		std::swap(readTex1, writeTex);
	}
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
