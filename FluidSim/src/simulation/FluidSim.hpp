#pragma once

#include <glm/glm.hpp>
#include "shader/ComputeShader.hpp"

class FluidSim {
public:
	FluidSim(int screenWidth, int screenHeight);
	~FluidSim();

	FluidSim()								= delete;
	FluidSim(const FluidSim&)				= delete;
	FluidSim& operator=(const FluidSim&)	= delete;
	FluidSim(FluidSim&&)					= delete;
	FluidSim& operator=(FluidSim&&)			= delete;

	void step(glm::vec2 mousePos, glm::vec2 mouseDir);

	GLuint generateTexture(int w, int h, GLenum internalFormat);

	GLuint getFinalTexture() const;

private:
	void addForce(glm::vec2 mousePos, glm::vec2 mouseForce, float newDens, float radius);
	void diffuse(GLuint& readTex, GLuint& writeTex, float diffuseCoeff);
	void project();
	void advect(GLuint& readTex, GLuint& writeTex, bool isFinalStep);
	void jacobiSolve(GLuint& readTex1, GLuint& readTex2, GLuint& writeTex, float a, float c);

	// constants used for projection
	static constexpr float PROJECT_A = 1.f;
	static constexpr float PROJECT_C = 4.f;

	// precalculated constants
	float m_DiffusionStep;
	float m_ViscosityStep;

	float m_ScreenWidth;
	float m_ScreenHeight;
	float m_DeltaTime;
	float m_JacobiIterations;
	// Velocity
	GLuint m_VelXTexture, m_VelXTextureNext;
	GLuint m_VelYTexture, m_VelYTextureNext;
	// Pressure
	GLuint m_PresTexture, m_PresTextureNext;
	// Divergence
	GLuint m_DivTexture;
	// Density
	GLuint m_DensTexture, m_DensTextureNext;
	// Compute Shader
	ComputeShader m_AddForceShader, m_AdvectShader, m_JacobiShader, m_ProjectShader, m_DivergenceShader;
};