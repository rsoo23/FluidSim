#pragma once

#include "shader/Shader.hpp"

class Application {
public:
	Application(int versionMajor, int versionMinor, int width, int height, std::string_view title);
	~Application();

	Application()								= delete;
	Application(const Application&)				= delete;
	Application& operator=(const Application&)	= delete;
	Application(Application&&)					= delete;
	Application& operator=(Application&&)		= delete;

	void run();

private:
	void processInput();
	GLFWwindow* m_Window;
};