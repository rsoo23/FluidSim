#include "pch.h"
#include "Application.hpp"
#include "shader/BaseShader.hpp"
#include "input/InputHandler.hpp"
#include "simulation/FluidSim.hpp"
#include "render/Renderer.hpp"

Application::Application(unsigned int versionMajor, unsigned int versionMinor, unsigned int width, unsigned int height, const std::string& title) :
	m_ScreenWidth{ width },
	m_ScreenHeight{ height }
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, versionMajor);  
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, versionMinor);  
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	m_Window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);

	if (m_Window == NULL)  
	{  
		std::cerr << "Failed to create GLFW window" << std::endl;  
		glfwTerminate();  
	}  

	glfwMakeContextCurrent(m_Window);  

	// Load OpenGL function pointers using GLAD  
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))  
	{  
		std::cerr << "Failed to initialize GLAD" << std::endl;  
		glfwTerminate();
	}

	glfwSetFramebufferSizeCallback(m_Window, framebufferSizeCallback);
};

void Application::framebufferSizeCallback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

void Application::run(BaseShader& vertShader, BaseShader& fragShader, FluidSim& fluidSim)
{
	Renderer renderer{ vertShader, fragShader };
	InputHandler inputHandler{};
	double deltaTime{ 0.f };
	double prevFrame{ 0.f };

	inputHandler.init(m_Window);
	while (!glfwWindowShouldClose(m_Window))
	{  
		double currFrame{ glfwGetTime() };
		deltaTime = currFrame - prevFrame;
		prevFrame = currFrame;

		const CursorState& cursorState{ inputHandler.getCursorState() };

		fluidSim.step(static_cast<float>(deltaTime), cursorState);

		GLuint finalTex{ fluidSim.getFinalTexture() };

		renderer.render(finalTex);

		// Swap buffers and poll events  
		glfwSwapBuffers(m_Window);  
		inputHandler.resetFrame();
		glfwPollEvents();  
	}  
}

Application::~Application()
{
	glfwTerminate();  
}
