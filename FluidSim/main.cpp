#include "pch.h"
#include "shader/Shader.hpp"
#include <filesystem>

void framebuffer_size_callback(GLFWwindow* window, int width, int height)  
{  
	glViewport(0, 0, width, height);  
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

int main()  
{  
	glfwInit();  
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);  
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);  
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  

	constexpr int width = 800, height = 600;

	// Correct the type name from GLFWWindow to GLFWwindow  
	GLFWwindow* window = glfwCreateWindow(width, height, "Learn OpenGL", NULL, NULL);  

	if (window == NULL)  
	{  
		std::cerr << "Failed to create GLFW window" << std::endl;  
		glfwTerminate();  
		return -1;  
	}  

	glfwMakeContextCurrent(window);  

	// Load OpenGL function pointers using GLAD  
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))  
	{  
		std::cerr << "Failed to initialize GLAD" << std::endl;  
		return -1;  
	}  

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	std::filesystem::path vertexRelPath = R"(shaders\shader.vert)";
	std::filesystem::path fragRelPath = R"(shaders\shader.frag)";
	std::filesystem::path cwd = std::filesystem::current_path();
	Shader shader(cwd / vertexRelPath, cwd / fragRelPath);

	float vertices[] = {
		// positions         // texture coords
		 0.5f,  0.5f, 0.0f,  1.0f, 1.0f,   // top right
		 0.5f, -0.5f, 0.0f,  1.0f, 0.0f,   // bottom right
		-0.5f, -0.5f, 0.0f,  0.0f, 0.0f,   // bottom left
		-0.5f,  0.5f, 0.0f,  0.0f, 1.0f    // top left 
	};

	unsigned int indices[] = {  
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };

	// Data
	constexpr int texWidth = 400, texHeight = 300;
	std::vector<uint8_t> pixels(texWidth * texHeight * 3, 128);

	// PBO
	//unsigned int PBO;
	//glGenBuffers(1, &PBO);
	//glBindBuffer(GL_PIXEL_PACK_BUFFER, PBO);

	// Texture
	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texWidth, texHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, pixels.data());

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// VAO, VBO
	unsigned int VAO, VBO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// Render loop  
	while (!glfwWindowShouldClose(window))  
	{  
		// input
		processInput(window);

		// rendering commands
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glBindTexture(GL_TEXTURE_2D, texture);

		shader.use();
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		// Swap buffers and poll events  
		glfwSwapBuffers(window);  
		glfwPollEvents();  
	}  

	glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

	glfwTerminate();  
	return 0;  
}
