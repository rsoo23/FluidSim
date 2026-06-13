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
	std::optional<glm::vec2> getMouseDragDir() const;

private:
	static void cursorPositionCallback(GLFWwindow* window, double xPos, double yPos);
	bool m_IsDragging;
	double m_MouseXPosPrev;
	double m_MouseYPosPrev;
	double m_MouseXPosCurr;
	double m_MouseYPosCurr;
	double m_MouseDirX;
	double m_MouseDirY;
};