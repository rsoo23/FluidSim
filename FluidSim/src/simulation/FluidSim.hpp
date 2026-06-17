#pragma once

#include <glm/glm.hpp>
#include "shader/ComputeShader.hpp"
#include "input/InputHandler.hpp"
#include "core/GLTexture.hpp"

class FluidSim {
public:
	FluidSim(
		unsigned int screenWidth,
		unsigned int screenHeight,
		unsigned int jacobiIterations,
		float diffusionCoeff,
		float viscosityCoeff,
		float cursorRadius,
		float vorticityCoeff,
		float forceMultiplier,
		float densityIncrement,
		float densityIncrementMultiplier
	);
	~FluidSim() = default;

	FluidSim()								= delete;
	FluidSim(const FluidSim&)				= delete;
	FluidSim& operator=(const FluidSim&)	= delete;
	FluidSim(FluidSim&&)					= delete;
	FluidSim& operator=(FluidSim&&)			= delete;

	void step(float deltaTime, const CursorState& cursorState);

	GLuint getFinalTexture() const;

private:
	void addForce(const CursorState& cursorState, float deltaTime);
	void curl();
	void vorticityConfine(float deltaTime);
	void diffuse(GLTexture& readTex, GLTexture& writeTex, float coeff, float deltaTime);
	void project();
	void advect(GLTexture& readTex, GLTexture& writeTex, float deltaTime, bool isFinalStep);
	void jacobiSolve(GLTexture& readTex1, GLTexture& readTex2, GLTexture& writeTex, float a, float c);

	// constants used for projection
	static constexpr float PROJECT_A{ 1.f };
	static constexpr float PROJECT_C{ 4.f };

	unsigned int m_ScreenWidth;
	unsigned int m_ScreenHeight;
	unsigned int m_JacobiIterations;
	float m_DiffusionCoeff;
	float m_ViscosityCoeff;
	float m_DensityIncrement;
	float m_CursorRadius;
	float m_ForceMultiplier;
	float m_VorticityCoeff;
	float m_DensityIncrementMultiplier;

	// Velocity
	GLTexture m_VelXTexture, m_VelXTextureNext;
	GLTexture m_VelYTexture, m_VelYTextureNext;
	// Pressure
	GLTexture m_PresTexture, m_PresTextureNext;
	// Divergence
	GLTexture m_DivTexture;
	// Density
	GLTexture m_DensTexture, m_DensTextureNext;
	// Curl 
	GLTexture m_CurlTexture;

	ComputeShader m_AddForceShader, m_AdvectShader, m_JacobiShader, m_ProjectShader, m_DivergenceShader, m_CurlShader, m_VorticityConfineShader;
};