#include "pch.h"
#include "Application.hpp"
#include "input/InputHandler.hpp"
#include "simulation/FluidSim.hpp"
#include "render/Renderer.hpp"

Application::Application(unsigned int versionMajor, unsigned int versionMinor, unsigned int width, unsigned int height, const std::string& title) :
	m_ScreenWidth{ width },
	m_ScreenHeight{ height }
{
	if (!glfwInit())
	{
		throw std::runtime_error("Failed to initialize GLFW");
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, versionMajor);  
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, versionMinor);  
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	m_Window = UniqueGLFWWindow(glfwCreateWindow(width, height, title.c_str(), NULL, NULL));

	if (!m_Window)
	{  
		throw std::runtime_error("Failed to create GLFW window");
	}

	glfwMakeContextCurrent(m_Window.get());

	// Load OpenGL function pointers using GLAD  
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))  
	{  
		throw std::runtime_error("Failed to initialize GLAD");
	}

	glfwSetFramebufferSizeCallback(m_Window.get(), framebufferSizeCallback);
};

void Application::framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void Application::run(const std::filesystem::path& vertexRelPath, const std::filesystem::path& fragRelPath, FluidSim& fluidSim)
{
	Renderer renderer{ vertexRelPath, fragRelPath };
	InputHandler inputHandler{};
	double deltaTime{ 0.f };
	double prevFrame{ 0.f };

	inputHandler.init(m_Window.get());
	while (!glfwWindowShouldClose(m_Window.get()))
	{  
		double currFrame{ glfwGetTime() };
		deltaTime = currFrame - prevFrame;
		prevFrame = currFrame;

		const CursorState& cursorState{ inputHandler.getCursorState() };

		fluidSim.step(static_cast<float>(deltaTime), cursorState);

		std::cout << "FPS: " << 1 / deltaTime << "\n";
		std::cout << cursorState.cursorDir.x << "\n";
		std::cout << cursorState.cursorDir.y << "\n\n";

		GLuint finalTex{ fluidSim.getFinalTexture() };

		renderer.render(finalTex);

		// Swap buffers and poll events  
		glfwSwapBuffers(m_Window.get());  
		inputHandler.resetFrame();
		glfwPollEvents();  
	}
}

Application::~Application()
{
	glfwTerminate();  
}
