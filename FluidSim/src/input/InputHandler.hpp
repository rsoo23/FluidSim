#pragma once

#include <glm/glm.hpp>
#include "shader/Shader.hpp"
#include "render/Renderer.hpp"

class InputHandler {
public:
	InputHandler();
	~InputHandler();

	InputHandler(const InputHandler&)				= delete;
	InputHandler& operator=(const InputHandler&)	= delete;
	InputHandler(InputHandler&&)					= delete;
	InputHandler& operator=(InputHandler&&)			= delete;

	void init(GLFWwindow* window);

	std::optional<glm::vec2> getMouseDragCoords() const;

private:
	static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
	static void cursorPositionCallback(GLFWwindow* window, double xPos, double yPos);
	bool m_IsDragging;
	double m_MouseXPos;
	double m_MouseYPos;
};