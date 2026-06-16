#pragma once

#include <filesystem>
#include "render/Renderer.hpp"
#include "simulation/FluidSim.hpp"

struct GLFWWindowDestructor
{
	void operator()(GLFWwindow* ptr)
	{
		glfwDestroyWindow(ptr);
	}
};

using UniqueGLFWWindow = std::unique_ptr<GLFWwindow, GLFWWindowDestructor>;

class Application {
public:
	Application(unsigned int versionMajor, unsigned int versionMinor, unsigned int width, unsigned int height, const std::string& title);
	~Application();

	Application()								= delete;
	Application(const Application&)				= delete;
	Application& operator=(const Application&)	= delete;
	Application(Application&&)					= delete;
	Application& operator=(Application&&)		= delete;

	void run(const std::filesystem::path& vertexRelPath, const std::filesystem::path& fragRelPath, FluidSim& fluidSim);

private:
	static void framebufferSizeCallback(GLFWwindow* window, int width, int height);
	UniqueGLFWWindow m_Window;
	unsigned int m_ScreenWidth;
	unsigned int m_ScreenHeight;
};