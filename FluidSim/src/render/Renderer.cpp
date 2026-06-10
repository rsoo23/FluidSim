#include "pch.h"
#include "Renderer.hpp"

Renderer::Renderer(
	const glm::vec4& bgColor,
	const std::vector<float>& vertices,
	const std::vector<unsigned int>& indices	
): m_BgColor(bgColor)
{
	// texture setup
	glGenTextures(1, &m_TextureId);
	glBindTexture(GL_TEXTURE_2D, m_TextureId);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 800, 600, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

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

void Renderer::render(Shader& shader, const TextureData& textureData)
{
	// clear bg
	glClearColor(m_BgColor.r, m_BgColor.g, m_BgColor.b, m_BgColor.a);
	glClear(GL_COLOR_BUFFER_BIT);

	// bind texture
	glBindTexture(GL_TEXTURE_2D, m_TextureId); 
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 800, 600, GL_RGB, GL_UNSIGNED_BYTE, textureData.pixels.data());

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
