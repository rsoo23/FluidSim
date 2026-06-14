#pragma once

#include <glm/glm.hpp>
#include "shader/Shader.hpp"
#include "render/Renderer.hpp"

struct CursorState
{
	glm::dvec2 cursorPosPrev;
	glm::dvec2 cursorPosCurr;
	glm::dvec2 cursorDir;
	bool isCursorInScreen;
};

class InputHandler {
public:
	InputHandler();

	~InputHandler()									= default;

	InputHandler(const InputHandler&)				= delete;
	InputHandler& operator=(const InputHandler&)	= delete;
	InputHandler(InputHandler&&)					= delete;
	InputHandler& operator=(InputHandler&&)			= delete;

	void init(GLFWwindow* window);

	void resetFrame();

	const CursorState& getCursorState() const;

private:
	static void cursorPositionCallback(GLFWwindow* window, double xPos, double yPos);
	static void cursorEnterCallback(GLFWwindow* window, int entered);
	CursorState m_CursorState;
};