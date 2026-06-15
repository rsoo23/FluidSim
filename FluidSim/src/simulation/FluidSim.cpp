#include "pch.h"
#include "FluidSim.hpp"
#include "shader/ComputeShader.hpp"

FluidSim::FluidSim(
	unsigned int screenWidth,
	unsigned int screenHeight,
	unsigned int jacobiIterations,
	float diffusionCoeff,
	float viscosityCoeff,
	float cursorRadius,
	float vorticityCoeff,
	float forceMultiplier,
	float densityIncrement
):
	m_ScreenWidth{ screenWidth },
	m_ScreenHeight{ screenHeight },
	m_JacobiIterations{ jacobiIterations },
	m_DiffusionCoeff{ diffusionCoeff },
	m_ViscosityCoeff{ viscosityCoeff },
	m_CursorRadius{ cursorRadius },
	m_VorticityCoeff{ vorticityCoeff },
	m_ForceMultiplier{ forceMultiplier },
	m_DensityIncrement{ densityIncrement },
	m_AddForceShader{ R"(shaders\addForce.comp)", screenWidth, screenHeight },
	m_AdvectShader{ R"(shaders\advect.comp)", screenWidth, screenHeight },
	m_JacobiShader{ R"(shaders\jacobi.comp)", screenWidth, screenHeight },
	m_ProjectShader{ R"(shaders\project.comp)", screenWidth, screenHeight },
	m_DivergenceShader{ R"(shaders\divergence.comp)", screenWidth, screenHeight },
	m_VorticityConfineShader{ R"(shaders\vorticityConfine.comp)", screenWidth, screenHeight },
	m_CurlShader{ R"(shaders\curl.comp)", screenWidth, screenHeight }
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

	m_CurlTexture		= generateTexture();

	// fill textures with 0
	setEmptyTexture(m_VelXTexture);
	setEmptyTexture(m_VelXTextureNext);
	setEmptyTexture(m_VelYTexture);
	setEmptyTexture(m_VelYTextureNext);
	setEmptyTexture(m_PresTexture);
	setEmptyTexture(m_PresTextureNext);
	setEmptyTexture(m_DivTexture);
	setEmptyTexture(m_DensTexture);
	setEmptyTexture(m_CurlTexture);
}

void FluidSim::step(float deltaTime, const CursorState& cursorState)
{
	// add forces
	addForce(cursorState, deltaTime);

	curl();
	vorticityConfine(deltaTime);

	// velocities:
	// diffuse
	diffuse(m_VelXTexture, m_VelXTextureNext, m_ViscosityCoeff, deltaTime);
	diffuse(m_VelYTexture, m_VelYTextureNext, m_ViscosityCoeff, deltaTime);
	// project
	project();
	// advect velocities
	advect(m_VelXTexture, m_VelXTextureNext, deltaTime, false);
	advect(m_VelYTexture, m_VelYTextureNext, deltaTime, false);
	// project
	project();

	// densities:
	// diffuse
	diffuse(m_DensTexture, m_DensTextureNext, m_DiffusionCoeff, deltaTime);
	// advect
	advect(m_DensTexture, m_DensTextureNext, deltaTime, true);
}

void FluidSim::addForce(const CursorState& cursorState, float deltaTime)
{
	m_AddForceShader.bindImageTexture(0, m_VelXTexture, GL_READ_WRITE, GL_R32F);
	m_AddForceShader.bindImageTexture(1, m_VelYTexture, GL_READ_WRITE, GL_R32F);
	m_AddForceShader.bindImageTexture(2, m_DensTexture, GL_READ_WRITE, GL_R32F);
	m_AddForceShader.use();
	m_AddForceShader.setVec2("cursorPos", glm::vec2(cursorState.cursorPosCurr));
	m_AddForceShader.setVec2("cursorForce", glm::vec2(cursorState.cursorDir));
	m_AddForceShader.setFloat("densityIncrement", cursorState.isCursorInScreen ? m_DensityIncrement : 0.f);
	m_AddForceShader.setFloat("cursorRadius", m_CursorRadius);
	m_AddForceShader.setFloat("forceMultiplier", m_ForceMultiplier);
	m_AddForceShader.setFloat("deltaTime", deltaTime);
	m_AddForceShader.dispatch();
}

void FluidSim::curl()
{
	m_CurlShader.bindImageTexture(0, m_VelXTexture, GL_READ_ONLY, GL_R32F);
	m_CurlShader.bindImageTexture(1, m_VelYTexture, GL_READ_ONLY, GL_R32F);
	m_CurlShader.bindImageTexture(2, m_CurlTexture, GL_WRITE_ONLY, GL_R32F);
	m_CurlShader.use();
	m_CurlShader.setUint("screenWidth", m_ScreenWidth);
	m_CurlShader.setUint("screenHeight", m_ScreenHeight);
	m_CurlShader.dispatch();
}

void FluidSim::vorticityConfine(float deltaTime)
{
	m_VorticityConfineShader.bindImageTexture(0, m_VelXTexture, GL_READ_WRITE, GL_R32F);
	m_VorticityConfineShader.bindImageTexture(1, m_VelYTexture, GL_READ_WRITE, GL_R32F);
	m_VorticityConfineShader.bindImageTexture(2, m_CurlTexture, GL_READ_ONLY, GL_R32F);
	m_VorticityConfineShader.use();
	m_VorticityConfineShader.setUint("screenWidth", m_ScreenWidth);
	m_VorticityConfineShader.setUint("screenHeight", m_ScreenHeight);
	m_VorticityConfineShader.setFloat("vorticity", m_VorticityCoeff);
	m_VorticityConfineShader.setFloat("deltaTime", deltaTime);
	m_VorticityConfineShader.dispatch();
}

void FluidSim::diffuse(GLuint& readTex, GLuint& writeTex, float coeff, float deltaTime)
{
	float a{ coeff * deltaTime };
	float c{ 1.f + 4.f * a };
	jacobiSolve(readTex, readTex, writeTex, a, c);
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
	jacobiSolve(m_PresTexture, m_DivTexture, m_PresTextureNext, PROJECT_A, PROJECT_C);
	// subtract pressure gradient for incompressibility
	m_ProjectShader.bindImageTexture(0, m_VelXTexture, GL_READ_WRITE, GL_R32F);
	m_ProjectShader.bindImageTexture(1, m_VelYTexture, GL_READ_WRITE, GL_R32F);
	m_ProjectShader.bindImageTexture(2, m_PresTexture, GL_READ_ONLY, GL_R32F);
	m_ProjectShader.use();
	m_ProjectShader.setUint("screenWidth", m_ScreenWidth);
	m_ProjectShader.setUint("screenHeight", m_ScreenHeight);
	m_ProjectShader.dispatch();
}

void FluidSim::advect(GLuint& readTex, GLuint& writeTex, float deltaTime, bool isFinalStep)
{
	m_AdvectShader.bindImageTexture(0, readTex, GL_READ_ONLY, GL_R32F);
	m_AdvectShader.bindImageTexture(1, writeTex, GL_WRITE_ONLY, GL_R32F);
	m_AdvectShader.bindImageTexture(2, m_VelXTexture, GL_READ_ONLY, GL_R32F);
	m_AdvectShader.bindImageTexture(3, m_VelYTexture, GL_READ_ONLY, GL_R32F);
	m_AdvectShader.use();
	m_AdvectShader.setUint("screenWidth", m_ScreenWidth);
	m_AdvectShader.setUint("screenHeight", m_ScreenHeight);
	m_AdvectShader.setFloat("deltaTime", deltaTime);
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
	for (unsigned int i{ 0 }; i < m_JacobiIterations; ++i)
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
		std::swap(readTex1, writeTex);
	}
}

GLuint FluidSim::generateTexture() const
{
	GLuint texId{};

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

FluidSim::~FluidSim()
{
	glDeleteTextures(1, &m_VelXTexture);
	glDeleteTextures(1, &m_VelXTextureNext);
	glDeleteTextures(1, &m_VelYTexture);
	glDeleteTextures(1, &m_VelYTextureNext);
	glDeleteTextures(1, &m_PresTexture);
	glDeleteTextures(1, &m_PresTextureNext);
	glDeleteTextures(1, &m_DivTexture);
	glDeleteTextures(1, &m_DensTexture);
	glDeleteTextures(1, &m_DensTextureNext);
	glDeleteTextures(1, &m_CurlTexture);
}
