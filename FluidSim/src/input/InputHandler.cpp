#include "pch.h"
#include "InputHandler.hpp"

InputHandler::InputHandler() : m_CursorState{ glm::dvec2{ 0, 0 }, glm::dvec2{ 0, 0 }, glm::dvec2{ 0, 0 }, false } {}

void InputHandler::init(GLFWwindow* window)
{
	glfwSetWindowUserPointer(window, this);
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	glfwSetCursorPosCallback(window, cursorPositionCallback);
	glfwSetCursorEnterCallback(window, cursorEnterCallback);
}

void InputHandler::resetFrame()
{
	m_CursorState.cursorDir.x = 0;
	m_CursorState.cursorDir.y = 0;
}

const CursorState& InputHandler::getCursorState() const
{
	return m_CursorState;
}

void InputHandler::cursorPositionCallback(GLFWwindow* window, double xPos, double yPos)
{
	auto* handler = static_cast<InputHandler*>(glfwGetWindowUserPointer(window));

	CursorState& state{ handler->m_CursorState };

	// calculate direction
	if (state.isCursorInScreen)
	{
		state.cursorPosCurr.x = xPos;
		state.cursorPosCurr.y = yPos;

		state.cursorDir.x = xPos - state.cursorPosPrev.x;
		state.cursorDir.y = yPos - state.cursorPosPrev.y;
	}
	state.cursorPosPrev.x = xPos;
	state.cursorPosPrev.y = yPos;
}

void InputHandler::cursorEnterCallback(GLFWwindow* window, int entered) {
	auto* handler = static_cast<InputHandler*>(glfwGetWindowUserPointer(window));

	CursorState& state{ handler->m_CursorState };

    if (entered) {
		state.isCursorInScreen = true;
    } else {
		state.isCursorInScreen = false;
    }
}
