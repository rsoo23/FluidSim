#include "pch.h"
#include "FluidSim.hpp"
#include "shader/ComputeShader.hpp"

FluidSim::FluidSim(unsigned int screenWidth, unsigned int screenHeight, unsigned int jacobiIterations):
	m_ScreenWidth(screenWidth),
	m_ScreenHeight(screenHeight),
	m_JacobiIterations(jacobiIterations),
	m_DeltaTime(1.f / 60.f),
	m_DiffusionStep(0.1f * m_DeltaTime),
	m_ViscosityStep(0.7f * m_DeltaTime)
{
	// compute shader textures setup
	m_VelXTexture		= generateTexture();
	m_VelXTextureNext	= generateTexture();
	m_VelYTexture		= generateTexture();
	m_VelYTextureNext	= generateTexture();

	m_PresTexture		= generateTexture();
	m_PresTextureNext	= generateTexture();

	m_DivTexture		= generateTexture();

	m_DensTexture		= generateTexture();
	m_DensTextureNext	= generateTexture();

	// fill textures with 0
	setEmptyTexture(m_VelXTexture);
	setEmptyTexture(m_VelXTextureNext);
	setEmptyTexture(m_VelYTexture);
	setEmptyTexture(m_VelYTextureNext);
	setEmptyTexture(m_PresTexture);
	setEmptyTexture(m_PresTextureNext);
	setEmptyTexture(m_DivTexture);
	setEmptyTexture(m_DensTexture);
	setEmptyTexture(m_DensTextureNext);

	// compute shader setup
	m_AddForceShader	= ComputeShader{ R"(shaders\addForce.comp)", m_ScreenWidth, m_ScreenHeight };
	m_AdvectShader		= ComputeShader{ R"(shaders\advect.comp)", m_ScreenWidth, m_ScreenHeight };
	m_JacobiShader		= ComputeShader{ R"(shaders\jacobi.comp)", m_ScreenWidth, m_ScreenHeight };
	m_ProjectShader		= ComputeShader{ R"(shaders\project.comp)", m_ScreenWidth, m_ScreenHeight };
	m_DivergenceShader	= ComputeShader{ R"(shaders\divergence.comp)", m_ScreenWidth, m_ScreenHeight };
}

void FluidSim::step(glm::vec2 mousePos, glm::vec2 mouseDir)
{
	// add forces
	addForce(mousePos, mouseDir, 0.25f, 10.f);

	// velocities:
	// diffuse
	diffuse(m_VelXTexture, m_VelXTextureNext, m_ViscosityStep);
	diffuse(m_VelYTexture, m_VelYTextureNext, m_ViscosityStep);
	// project
	project();
	// advect velocities
	advect(m_VelXTexture, m_VelXTextureNext, false);
	advect(m_VelYTexture, m_VelYTextureNext, false);
	// project
	project();

	// densities:
	// diffuse
	diffuse(m_DensTexture, m_DensTextureNext, m_DiffusionStep);
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

// jacobi(vel1, velnext, vel2)
// b, x, xtemp
// xtemp = b + a * (x);
// vel2 = vel1 + a * velnext
// swap velnext and vel2
// x

void FluidSim::diffuse(GLuint& readTex, GLuint& writeTex, float diffuseCoeff)
{
	jacobiSolve(readTex, readTex, writeTex, diffuseCoeff, 1.f + 4.f * diffuseCoeff);
}

void FluidSim::project()
{
	// divergence
	m_DivergenceShader.bindImageTexture(0, m_VelXTexture, GL_READ_ONLY, GL_R32F);
	m_DivergenceShader.bindImageTexture(1, m_VelYTexture, GL_READ_ONLY, GL_R32F);
	m_DivergenceShader.bindImageTexture(2, m_DivTexture, GL_WRITE_ONLY, GL_R32F);
	m_DivergenceShader.use();
	m_DivergenceShader.setUint("screenWidth", m_ScreenWidth);
	m_DivergenceShader.setUint("screenHeight", m_ScreenHeight);
	m_DivergenceShader.dispatch();
	// solve pressure poisson
	jacobiSolve(m_DivTexture, m_PresTexture, m_PresTextureNext, PROJECT_A, PROJECT_C);
	// subtract pressure gradient for incompressibility
	m_ProjectShader.bindImageTexture(0, m_VelXTexture, GL_READ_WRITE, GL_R32F);
	m_ProjectShader.bindImageTexture(1, m_VelYTexture, GL_READ_WRITE, GL_R32F);
	m_ProjectShader.bindImageTexture(2, m_PresTexture, GL_READ_ONLY, GL_R32F);
	m_ProjectShader.use();
	m_ProjectShader.setUint("screenWidth", m_ScreenWidth);
	m_ProjectShader.setUint("screenHeight", m_ScreenHeight);
	m_ProjectShader.dispatch();
}

void FluidSim::advect(GLuint& readTex, GLuint& writeTex, bool isFinalStep)
{
	m_AdvectShader.bindImageTexture(0, readTex, GL_READ_ONLY, GL_R32F);
	m_AdvectShader.bindImageTexture(1, writeTex, GL_WRITE_ONLY, GL_R32F);
	m_AdvectShader.bindImageTexture(2, m_VelXTexture, GL_READ_ONLY, GL_R32F);
	m_AdvectShader.bindImageTexture(3, m_VelYTexture, GL_READ_ONLY, GL_R32F);
	m_AdvectShader.use();
	m_AdvectShader.setUint("screenWidth", m_ScreenWidth);
	m_AdvectShader.setUint("screenHeight", m_ScreenHeight);
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

void FluidSim::jacobiSolve(GLuint& readTex1, GLuint& readTex2, GLuint& writeTex, float a, float c)
{
	for (int i = 0; i < m_JacobiIterations; ++i)
	{
		m_JacobiShader.bindImageTexture(0, readTex1, GL_READ_ONLY, GL_R32F);
		m_JacobiShader.bindImageTexture(1, readTex2, GL_READ_ONLY, GL_R32F);
		m_JacobiShader.bindImageTexture(2, writeTex, GL_WRITE_ONLY, GL_R32F);
		m_JacobiShader.use();
		m_JacobiShader.setFloat("a", a);
		m_JacobiShader.setFloat("c", c);
		m_JacobiShader.setUint("screenWidth", m_ScreenWidth);
		m_JacobiShader.setUint("screenHeight", m_ScreenHeight);
		m_JacobiShader.dispatch();
		std::swap(readTex2, writeTex);
	}
}

GLuint FluidSim::generateTexture()
{
	GLuint texId;

	glCreateTextures(GL_TEXTURE_2D, 1, &texId);
	glTextureStorage2D(texId, 1, GL_R32F, static_cast<GLsizei>(m_ScreenWidth), static_cast<GLsizei>(m_ScreenHeight));

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	return texId;
}

void FluidSim::setEmptyTexture(GLuint texId)
{
	glClearTexImage(texId, 0, GL_RED, GL_FLOAT, NULL);
}

GLuint FluidSim::getFinalTexture() const
{
	return m_DensTexture;
}
