#include "pch.h"
#include "InputHandler.hpp"


InputHandler::InputHandler() :
	m_IsCursorInScreen(false),
	m_IsCursorMoving(false),
	m_MouseXPosPrev(0),
	m_MouseYPosPrev(0),
	m_MouseXPosCurr(0),
	m_MouseYPosCurr(0),
	m_MouseDirX(0),
	m_MouseDirY(0) {}

void InputHandler::init(GLFWwindow* window)
{
	glfwSetWindowUserPointer(window, this);
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	glfwSetCursorPosCallback(window, cursorPositionCallback);
	glfwSetCursorEnterCallback(window, cursorEnterCallback);
}

glm::vec2 InputHandler::getMouseDragCoords() const
{
	return glm::vec2(m_MouseXPosCurr, m_MouseYPosCurr);
}

glm::vec2 InputHandler::getMouseDragDir() const
{
	return glm::vec2(m_MouseDirX, m_MouseDirY);
}

void InputHandler::resetFrame()
{
	m_IsCursorMoving = false;
	m_MouseDirX = 0;
	m_MouseDirY = 0;
}

bool InputHandler::isCursorInScreen() const
{
	return m_IsCursorInScreen;
}

bool InputHandler::isCursorMoving() const
{
	return m_IsCursorMoving;
}

void InputHandler::cursorPositionCallback(GLFWwindow* window, double xPos, double yPos)
{
	auto* handler = static_cast<InputHandler*>(glfwGetWindowUserPointer(window));

	handler->m_IsCursorMoving = true;

	// calculate direction
	if (handler->m_IsCursorInScreen)
	{
		handler->m_MouseXPosCurr = xPos;
		handler->m_MouseYPosCurr = yPos;

		handler->m_MouseDirX = xPos - handler->m_MouseXPosPrev;
		handler->m_MouseDirY = yPos - handler->m_MouseYPosPrev;
	}
	handler->m_MouseXPosPrev = xPos;
	handler->m_MouseYPosPrev = yPos;
}

void InputHandler::cursorEnterCallback(GLFWwindow* window, int entered) {
	auto* handler = static_cast<InputHandler*>(glfwGetWindowUserPointer(window));
    if (entered) {
		handler->m_IsCursorInScreen = true;
    } else {
		handler->m_IsCursorInScreen = false;
    }
}
