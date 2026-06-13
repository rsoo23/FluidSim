#include "pch.h"
#include "InputHandler.hpp"


InputHandler::InputHandler() :
	m_IsDragging(true),
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
	glfwSetMouseButtonCallback(window, mouseButtonCallback);
}

std::optional<glm::vec2> InputHandler::getMouseDragCoords() const
{
	if (m_IsDragging)
	{
		return glm::vec2(m_MouseXPosCurr, m_MouseYPosCurr);
	}
	return std::nullopt;
}

std::optional<glm::vec2> InputHandler::getMouseDragDir() const
{
	if (m_IsDragging)
	{
		return glm::vec2(m_MouseDirX, m_MouseDirY);
	}
	return std::nullopt;
}

void InputHandler::cursorPositionCallback(GLFWwindow* window, double xPos, double yPos)
{
	auto* handler = static_cast<InputHandler*>(glfwGetWindowUserPointer(window));
	if (handler->m_IsDragging)
	{
		handler->m_MouseXPosCurr = xPos;
		handler->m_MouseYPosCurr = yPos;

		handler->m_MouseDirX = xPos - handler->m_MouseXPosPrev;
		handler->m_MouseDirY = yPos - handler->m_MouseYPosPrev;
	}
	handler->m_MouseXPosPrev = xPos;
	handler->m_MouseYPosPrev = yPos;
}
	}
}

InputHandler::~InputHandler() {}
