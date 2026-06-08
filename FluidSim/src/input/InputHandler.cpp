#include "pch.h"
#include "InputHandler.hpp"


InputHandler::InputHandler() : m_IsDragging(false), m_MouseXPos(0), m_MouseYPos(0) {}

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
		return glm::vec2(m_MouseXPos, m_MouseYPos);
	}
	return std::nullopt;
}

void InputHandler::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
	auto* handler = static_cast<InputHandler*>(glfwGetWindowUserPointer(window));
	if (button == GLFW_MOUSE_BUTTON_LEFT)
	{
		if (action == GLFW_PRESS)
		{
			handler->m_IsDragging = true;
		}
		else if (action == GLFW_RELEASE)
		{
			handler->m_IsDragging = false;
		}
	}
}

void InputHandler::cursorPositionCallback(GLFWwindow* window, double xPos, double yPos)
{
	auto* handler = static_cast<InputHandler*>(glfwGetWindowUserPointer(window));
	if (handler->m_IsDragging)
	{
		glfwGetCursorPos(window, &handler->m_MouseXPos, &handler->m_MouseYPos);
	}
}

InputHandler::~InputHandler() {}
