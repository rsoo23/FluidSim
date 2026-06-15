#include "pch.h"
#include "Renderer.hpp"
#include "shader/BaseShader.hpp"

Renderer::Renderer(const std::filesystem::path& vertexRelPath, const std::filesystem::path& fragRelPath) :
	m_VertShader{ vertexRelPath, GL_VERTEX_SHADER },
	m_FragShader{ fragRelPath, GL_FRAGMENT_SHADER }
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
	glGenVertexArrays(1, &m_VAO);
	glGenBuffers(1, &m_VBO);
	glGenBuffers(1, &m_EBO);

	glBindVertexArray(m_VAO);

	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

	// Vertex Attributes
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
}

void Renderer::render(GLuint finalTexture)
{
	// clear bg
	glClearColor(0.f, 0.f, 0.f, 0.f);
	glClear(GL_COLOR_BUFFER_BIT);

	// bind texture
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, finalTexture); 

	// user shaders
	m_VertShader.use();
	m_FragShader.use();

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
