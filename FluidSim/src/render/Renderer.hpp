#pragma once

#include <glm/glm.hpp>
#include "shader/BaseShader.hpp"

class Renderer {
public:
	Renderer(const std::filesystem::path& vertexRelPath, const std::filesystem::path& fragRelPath);
	~Renderer();

	Renderer(const Renderer&)				= delete;
	Renderer& operator=(const Renderer&)	= delete;
	Renderer(Renderer&&)					= delete;
	Renderer& operator=(Renderer&&)			= delete;

	void render(GLuint finalTexture);

private:
	BaseShader m_VertShader, m_FragShader;
	GLuint m_VBO, m_VAO, m_EBO;
};