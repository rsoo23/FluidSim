#include "pch.h"
#include "Application.hpp"
#include "shader/Shader.hpp"

namespace {
	void framebuffer_size_callback(GLFWwindow* window, int width, int height)
	{
		glViewport(0, 0, width, height);
	};

	bool isDragging = false;
	double xPos = 0;
	double yPos = 0;

	void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
	{
		if (button == GLFW_MOUSE_BUTTON_LEFT)
		{
			if (action == GLFW_PRESS)
			{
				isDragging = true;
			}
			else if (action == GLFW_RELEASE)
			{
				isDragging = false;
			}
		}
	}

	void cursor_position_callback(GLFWwindow* window, double xPos, double yPos)
	{
		if (isDragging)
		{
			glfwGetCursorPos(window, &xPos, &yPos);
			std::cout << "x: " << xPos << ", y: " << yPos << "\n";
		}
	}
}

Application::Application(int versionMajor, int versionMinor, int width, int height, std::string_view title)
{
	glfwInit();  
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, versionMajor);  
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, versionMinor);  
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  

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

	glfwSetFramebufferSizeCallback(m_Window, framebuffer_size_callback);
};

void Application::run(Shader& shader, Renderer& renderer)
{
	while (!glfwWindowShouldClose(m_Window))  
	{  
		// input
		processInput();

		renderer.render(shader);

		// Swap buffers and poll events  
		glfwSwapBuffers(m_Window);  
		glfwPollEvents();  
	}  
}

void Application::processInput()
{
	if (glfwGetKey(m_Window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(m_Window, true);

	glfwSetCursorPosCallback(m_Window, cursor_position_callback);
	glfwSetMouseButtonCallback(m_Window, mouse_button_callback);
}

Application::~Application()
{
	glfwTerminate();  
}
