#pragma once

class FluidSim {
public:
	FluidSim(int screenWidth, int screenHeight);
	~FluidSim();

	FluidSim()								= delete;
	FluidSim(const FluidSim&)				= delete;
	FluidSim& operator=(const FluidSim&)	= delete;
	FluidSim(FluidSim&&)					= delete;
	FluidSim& operator=(FluidSim&&)			= delete;

	void step();

	GLuint generateTexture(int w, int h, GLint internalFormat, GLenum texImage2DFormat, GLuint unit, GLenum bindImageTexFormat);

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
};