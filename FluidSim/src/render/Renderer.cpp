#include "pch.h"
#include "Renderer.hpp"

Renderer::Renderer(
	const glm::vec4& bgColor,
	const std::vector<float>& vertices,
	const std::vector<unsigned int>& indices	
): m_BgColor(bgColor)
{
	// VAO, VBO, EBO Setup
	glGenVertexArrays(1, &m_VAO);
	glGenBuffers(1, &m_VBO);
	glGenBuffers(1, &m_EBO);

	glBindVertexArray(m_VAO);

	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(float), indices.data(), GL_STATIC_DRAW);

	// Vertex Attributes
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
}

void Renderer::render(Shader& shader, GLuint finalTexture)
{
	// clear bg
	glClearColor(m_BgColor.r, m_BgColor.g, m_BgColor.b, m_BgColor.a);
	glClear(GL_COLOR_BUFFER_BIT);

	// bind texture
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, finalTexture); 

	// user shaders
	shader.use();

	// bind VAO
	glBindVertexArray(m_VAO);

	// draw
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

Renderer::~Renderer()
{
	glDeleteVertexArrays(1, &m_VAO);
    glDeleteBuffers(1, &m_VBO);
    glDeleteBuffers(1, &m_EBO);
}
