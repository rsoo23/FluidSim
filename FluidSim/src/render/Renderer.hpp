#pragma once

#include <glm/glm.hpp>
#include "shader/Shader.hpp"

class Renderer {
public:
	Renderer();
	~Renderer();

	Renderer(const Renderer&)				= delete;
	Renderer& operator=(const Renderer&)	= delete;
	Renderer(Renderer&&)					= delete;
	Renderer& operator=(Renderer&&)			= delete;

	void render(Shader& shader, GLuint finalTexture);

private:
	GLuint m_VBO, m_VAO, m_EBO;
};