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

void Application::run(Shader& shader, Renderer& renderer, FluidSim& fluidSim, float densityIncrement)
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

		bool isCursorInScreen = inputHandler.isCursorInScreen();
		bool isCursorMoving = inputHandler.isCursorMoving();

		glm::vec2 mousePos = inputHandler.getMouseDragCoords();
		glm::vec2 mouseDragDir = inputHandler.getMouseDragDir();

		float finalDensityIncrement = isCursorInScreen ? densityIncrement : 0;

		std::cout << "cursor move?" << isCursorMoving << "\n";
		std::cout << "cursor in screen?" << isCursorInScreen << "\n";
		std::cout << "x: " << mousePos.x << ", y: " << mousePos.y << "\n";
		std::cout << "dirx: " << mouseDragDir.x << ", diry: " << mouseDragDir.y << "\n\n";

		fluidSim.setDensityIncrement(finalDensityIncrement);
		fluidSim.step(static_cast<float>(deltaTime), mousePos, mouseDragDir);

		GLuint finalTex = fluidSim.getFinalTexture();
		renderer.render(shader, finalTex);

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
