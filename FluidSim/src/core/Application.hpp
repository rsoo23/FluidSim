#pragma once

#include "shader/Shader.hpp"
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

	void run(Shader& shader, Renderer& renderer, FluidSim& fluidSim);

private:
	static void framebufferSizeCallback(GLFWwindow* window, int width, int height);
	GLFWwindow* m_Window;
	unsigned int m_ScreenWidth;
	unsigned int m_ScreenHeight;
};