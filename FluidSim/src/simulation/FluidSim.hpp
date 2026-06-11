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

	void step(glm::vec2 mousePos);

	GLuint generateTexture(int w, int h, GLenum internalFormat);

	GLuint getFinalTexture() const;

private:
	int m_ScreenWidth;
	int m_ScreenHeight;
	float m_DiffuseFactor;
	float m_Viscosity;
	float m_DeltaTime;
	// Velocity
	GLuint m_VelTexture, m_VelTextureNext;
	// Pressure
	GLuint m_PresTexture, m_PresTextureNext;
	// Divergence
	GLuint m_DivTexture;
	// Density
	GLuint m_DensTexture, m_DensTextureNext;
	// Compute Shader
	ComputeShader m_AddForceShader, m_AdvectShader, m_DiffuseShader, m_JacobiShader, m_ProjectShader;
};