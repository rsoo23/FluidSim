#pragma once

#include <glm/glm.hpp>
#include "shader/Shader.hpp"
#include "render/Renderer.hpp"

class InputHandler {
public:
	InputHandler();

	~InputHandler()									= default;

	InputHandler(const InputHandler&)				= delete;
	InputHandler& operator=(const InputHandler&)	= delete;
	InputHandler(InputHandler&&)					= delete;
	InputHandler& operator=(InputHandler&&)			= delete;

	void init(GLFWwindow* window);

	glm::vec2 getMouseDragCoords() const;
	glm::vec2 getMouseDragDir() const;

	void resetFrame();

	bool isCursorInScreen() const;

private:
	static void cursorPositionCallback(GLFWwindow* window, double xPos, double yPos);
	static void cursorEnterCallback(GLFWwindow* window, int entered);
	bool m_IsCursorInScreen;
	bool m_IsCursorMoving;
	double m_MouseXPosPrev;
	double m_MouseYPosPrev;
	double m_MouseXPosCurr;
	double m_MouseYPosCurr;
	double m_MouseDirX;
	double m_MouseDirY;
};