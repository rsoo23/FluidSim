#include "pch.h"
#include "Application.hpp"
#include "shader/Shader.hpp"
#include "input/InputHandler.hpp"
#include "simulation/FluidSim.hpp"

Application::Application(int versionMajor, int versionMinor, int width, int height, std::string_view title) : m_ScreenWidth(width), m_ScreenHeight(height)
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, versionMajor);  
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, versionMinor);  
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	m_Window = glfwCreateWindow(width, height, "FluidSim", NULL, NULL);  

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

void Application::run(Shader& shader, Renderer& renderer, InputHandler& inputHandler, FluidSim& fluidSim, TextureData& textureData)
{
	inputHandler.init(m_Window);
	while (!glfwWindowShouldClose(m_Window))  
	{  
		auto mouseDragCoords = inputHandler.getMouseDragCoords();
		if (mouseDragCoords) {
			auto mouseX = (*mouseDragCoords).x;
			auto mouseY = (*mouseDragCoords).y;
			auto pixelCoord = ((mouseY * m_ScreenWidth) + mouseX) * 3;
			//std::cout << "x: " << mouseX << ", y: " << mouseY << "\n";
			//std::cout << "convert: " << pixelCoord << "\n";
			textureData.pixels[pixelCoord] = 0;
			textureData.pixels[pixelCoord + 1] = 0;
			textureData.pixels[pixelCoord + 2] = 128;
		}
		//fluidSim.step();
		renderer.render(shader, textureData);

		// Swap buffers and poll events  
		glfwSwapBuffers(m_Window);  
		glfwPollEvents();  
	}  
}

Application::~Application()
{
	glfwTerminate();  
}
