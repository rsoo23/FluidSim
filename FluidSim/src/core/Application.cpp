#include "pch.h"
#include "Application.hpp"
#include "shader/Shader.hpp"
#include "input/InputHandler.hpp"
#include "simulation/FluidSim.hpp"

Application::Application(unsigned int versionMajor, unsigned int versionMinor, unsigned int width, unsigned int height, const std::string& title) :
	m_ScreenWidth(width),
	m_ScreenHeight(height)
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

void Application::run(Shader& shader, Renderer& renderer, FluidSim& fluidSim)
{
	InputHandler inputHandler;
	GLfloat deltaTime{ 0.f };
	GLfloat prevFrame{ 0.f };

	inputHandler.init(m_Window);
	while (!glfwWindowShouldClose(m_Window))  
	{  
		GLfloat currFrame = glfwGetTime();
		deltaTime = currFrame - prevFrame;
		prevFrame = currFrame;

		std::optional<glm::vec2> mouseDragCoords = inputHandler.getMouseDragCoords();
		std::optional<glm::vec2> mouseDragDir = inputHandler.getMouseDragDir();

		if (mouseDragCoords && mouseDragDir) {
			float mouseX = (*mouseDragCoords).x;
			float mouseY = (*mouseDragCoords).y;

			float mouseDirX = (*mouseDragDir).x;
			float mouseDirY = (*mouseDragDir).y;

			std::cout << "delt" << deltaTime << "\n";
			std::cout << "x: " << mouseX << ", y: " << mouseY << "\n";
			std::cout << "dirx: " << mouseDirX << ", diry: " << mouseDirY << "\n\n";

			fluidSim.step(static_cast<float>(deltaTime), *mouseDragCoords, *mouseDragDir);

			GLuint finalTex = fluidSim.getFinalTexture();
			renderer.render(shader, finalTex);
		}

		// Swap buffers and poll events  
		glfwSwapBuffers(m_Window);  
		glfwPollEvents();  
	}  
}

Application::~Application()
{
	glfwTerminate();  
}
