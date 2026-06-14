#pragma once

#include "shader/BaseShader.hpp"
#include "render/Renderer.hpp"
#include "input/InputHandler.hpp"
#include "simulation/FluidSim.hpp"

class Application {
public:
	Application(unsigned int versionMajor, unsigned int versionMinor, unsigned int width, unsigned int height, const std::string& title);
	~Application();

	Application()								= delete;
	Application(const Application&)				= delete;
	Application& operator=(const Application&)	= delete;
	Application(Application&&)					= delete;
	Application& operator=(Application&&)		= delete;

	void run(BaseShader& vertShader, BaseShader& fragShader, FluidSim& fluidSim);

private:
	static void framebufferSizeCallback(GLFWwindow* window, int width, int height);
	GLFWwindow* m_Window;
	unsigned int m_ScreenWidth;
	unsigned int m_ScreenHeight;
};