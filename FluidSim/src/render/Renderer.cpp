#include "pch.h"
#include "Renderer.hpp"

Renderer::Renderer(const std::filesystem::path& vertexRelPath, const std::filesystem::path& fragRelPath) :
	m_VertFragShader{ vertexRelPath, fragRelPath }
{
	static constexpr std::array<float, 20> vertices{
		// positions     // texture coords
		 1.f,  1.f, 0.f, 1.f, 0.f,   // top right
		 1.f, -1.f, 0.f, 1.f, 1.f,   // bottom right
		-1.f, -1.f, 0.f, 0.f, 1.f,   // bottom left
		-1.f,  1.f, 0.f, 0.f, 0.f    // top left 
	};

	static constexpr std::array<unsigned int, 6> indices{
		0, 1, 3, // first triangle
		1, 2, 3  // second triangle
	};

	// VAO, VBO, EBO Setup
	glCreateVertexArrays(1, &m_VAO);
	glCreateBuffers(1, &m_VBO);
	glCreateBuffers(1, &m_EBO);

	glNamedBufferStorage(m_VBO, vertices.size() * sizeof(float), vertices.data(), 0);

	glNamedBufferStorage(m_EBO, indices.size() * sizeof(unsigned int), indices.data(), 0);
	glVertexArrayElementBuffer(m_VAO, m_EBO);

	// Vertex Attributes
	glVertexArrayVertexBuffer(m_VAO, 0, m_VBO, 0, 5 * sizeof(float));

	glVertexArrayAttribFormat(m_VAO, 0, 3, GL_FLOAT, GL_FALSE, 0);
	glVertexArrayAttribFormat(m_VAO, 1, 2, GL_FLOAT, GL_FALSE, 3 * sizeof(float));

	glVertexArrayAttribBinding(m_VAO, 0, 0);
	glVertexArrayAttribBinding(m_VAO, 1, 0);

	glEnableVertexArrayAttrib(m_VAO, 0);
	glEnableVertexArrayAttrib(m_VAO, 1);
}

void Renderer::render(GLuint finalTexture)
{
	// clear bg
	glClearColor(0.f, 0.f, 0.f, 0.f);
	glClear(GL_COLOR_BUFFER_BIT);

	// bind texture
	glBindTextureUnit(0, finalTexture); 

	// user shaders
	m_VertFragShader.use();

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
	m_VAO = m_VBO = m_EBO = 0;
}
